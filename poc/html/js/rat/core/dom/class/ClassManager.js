/**
 * @author Jacky Nguyen <jacky@sencha.com>
 * @docauthor Jacky Nguyen <jacky@sencha.com>
 * @class Rat.ClassManager
 *
 * Rat.ClassManager manages all classes and handles mapping from string class name to
 * actual class objects throughout the whole framework. It is not generally accessed directly, rather through
 * these convenient shorthands:
 *
 * - {@link Rat#define Rat.define}
 * - {@link Rat#create Rat.create}
 * - {@link Rat#widget Rat.widget}
 * - {@link Rat#getClass Rat.getClass}
 * - {@link Rat#getClassName Rat.getClassName}
 *
 * # Basic syntax:
 *
 *     Rat.define(className, properties);
 *
 * in which `properties` is an object represent a collection of properties that apply to the class. See
 * {@link Rat.ClassManager#create} for more detailed instructions.
 *
 *     Rat.define('Person', {
 *          name: 'Unknown',
 *
 *          constructor: function(name) {
 *              if (name) {
 *                  this.name = name;
 *              }
 *          },
 *
 *          eat: function(foodType) {
 *              alert("I'm eating: " + foodType);
 *
 *              return this;
 *          }
 *     });
 *
 *     var aaron = new Person("Aaron");
 *     aaron.eat("Sandwich"); // alert("I'm eating: Sandwich");
 *
 * Rat.Class has a powerful set of extensible {@link Rat.Class#registerPreprocessor pre-processors} which takes care of
 * everything related to class creation, including but not limited to inheritance, mixins, configuration, statics, etc.
 *
 * # Inheritance:
 *
 *     Rat.define('Developer', {
 *          extend: 'Person',
 *
 *          constructor: function(name, isGeek) {
 *              this.isGeek = isGeek;
 *
 *              // Apply a method from the parent class' prototype
 *              this.callParent([name]);
 *          },
 *
 *          code: function(language) {
 *              alert("I'm coding in: " + language);
 *
 *              this.eat("Bugs");
 *
 *              return this;
 *          }
 *     });
 *
 *     var jacky = new Developer("Jacky", true);
 *     jacky.code("JavaScript"); // alert("I'm coding in: JavaScript");
 *                               // alert("I'm eating: Bugs");
 *
 * See {@link Rat.Base#callParent} for more details on calling superclass' methods
 *
 * # Mixins:
 *
 *     Rat.define('CanPlayGuitar', {
 *          playGuitar: function() {
 *             alert("F#...G...D...A");
 *          }
 *     });
 *
 *     Rat.define('CanComposeSongs', {
 *          composeSongs: function() { ... }
 *     });
 *
 *     Rat.define('CanSing', {
 *          sing: function() {
 *              alert("I'm on the highway to hell...")
 *          }
 *     });
 *
 *     Rat.define('Musician', {
 *          extend: 'Person',
 *
 *          mixins: {
 *              canPlayGuitar: 'CanPlayGuitar',
 *              canComposeSongs: 'CanComposeSongs',
 *              canSing: 'CanSing'
 *          }
 *     })
 *
 *     Rat.define('CoolPerson', {
 *          extend: 'Person',
 *
 *          mixins: {
 *              canPlayGuitar: 'CanPlayGuitar',
 *              canSing: 'CanSing'
 *          },
 *
 *          sing: function() {
 *              alert("Ahem....");
 *
 *              this.mixins.canSing.sing.call(this);
 *
 *              alert("[Playing guitar at the same time...]");
 *
 *              this.playGuitar();
 *          }
 *     });
 *
 *     var me = new CoolPerson("Jacky");
 *
 *     me.sing(); // alert("Ahem...");
 *                // alert("I'm on the highway to hell...");
 *                // alert("[Playing guitar at the same time...]");
 *                // alert("F#...G...D...A");
 *
 * # Config:
 *
 *     Rat.define('SmartPhone', {
 *          config: {
 *              hasTouchScreen: false,
 *              operatingSystem: 'Other',
 *              price: 500
 *          },
 *
 *          isExpensive: false,
 *
 *          constructor: function(config) {
 *              this.initConfig(config);
 *          },
 *
 *          applyPrice: function(price) {
 *              this.isExpensive = (price > 500);
 *
 *              return price;
 *          },
 *
 *          applyOperatingSystem: function(operatingSystem) {
 *              if (!(/^(iOS|Android|BlackBerry)$/i).test(operatingSystem)) {
 *                  return 'Other';
 *              }
 *
 *              return operatingSystem;
 *          }
 *     });
 *
 *     var iPhone = new SmartPhone({
 *          hasTouchScreen: true,
 *          operatingSystem: 'iOS'
 *     });
 *
 *     iPhone.getPrice(); // 500;
 *     iPhone.getOperatingSystem(); // 'iOS'
 *     iPhone.getHasTouchScreen(); // true;
 *     iPhone.hasTouchScreen(); // true
 *
 *     iPhone.isExpensive; // false;
 *     iPhone.setPrice(600);
 *     iPhone.getPrice(); // 600
 *     iPhone.isExpensive; // true;
 *
 *     iPhone.setOperatingSystem('AlienOS');
 *     iPhone.getOperatingSystem(); // 'Other'
 *
 * # Statics:
 *
 *     Rat.define('Computer', {
 *          statics: {
 *              factory: function(brand) {
 *                 // 'this' in static methods refer to the class itself
 *                  return new this(brand);
 *              }
 *          },
 *
 *          constructor: function() { ... }
 *     });
 *
 *     var dellComputer = Computer.factory('Dell');
 *
 * Also see {@link Rat.Base#statics} and {@link Rat.Base#self} for more details on accessing
 * static properties within class methods
 *
 * @singleton
 */
(function(Class, alias, arraySlice, arrayFrom, global) {

    var Manager = Rat.ClassManager = {

        /**
         * @property {Object} classes
         * All classes which were defined through the ClassManager. Keys are the
         * name of the classes and the values are references to the classes.
         * @private
         */
        classes: {},

        /**
         * @private
         */
        existCache: {},

        /**
         * @private
         */
        namespaceRewrites: [{
            from: 'Rat.',
            to: Rat
        }],

        /**
         * @private
         */
        maps: {
            alternateToName: {},
            aliasToName: {},
            nameToAliases: {},
            nameToAlternates: {}
        },

        /** @private */
        enableNamespaceParseCache: true,

        /** @private */
        namespaceParseCache: {},

        /** @private */
        instantiators: [],

        /**
         * Checks if a class has already been created.
         *
         * @param {String} className
         * @return {Boolean} exist
         */
        isCreated: function(className) {
            var existCache = this.existCache,
                i, ln, part, root, parts;

            //<debug error>
            if (typeof className != 'string' || className.length < 1) {
                throw new Error("[Rat.ClassManager] Invalid classname, must be a string and must not be empty");
            }
            //</debug>

            if (this.classes[className] || existCache[className]) {
                return true;
            }

            root = global;
            parts = this.parseNamespace(className);

            for (i = 0, ln = parts.length; i < ln; i++) {
                part = parts[i];

                if (typeof part != 'string') {
                    root = part;
                } else {
                    if (!root || !root[part]) {
                        return false;
                    }

                    root = root[part];
                }
            }

            existCache[className] = true;

            this.triggerCreated(className);

            return true;
        },

        /**
         * @private
         */
        createdListeners: [],

        /**
         * @private
         */
        nameCreatedListeners: {},

        /**
         * @private
         */
        triggerCreated: function(className) {
            var listeners = this.createdListeners,
                nameListeners = this.nameCreatedListeners,
                alternateNames = this.maps.nameToAlternates[className],
                names = [className],
                i, ln, j, subLn, listener, name;

            for (i = 0,ln = listeners.length; i < ln; i++) {
                listener = listeners[i];
                listener.fn.call(listener.scope, className);
            }

            if (alternateNames) {
                names.push.apply(names, alternateNames);
            }

            for (i = 0,ln = names.length; i < ln; i++) {
                name = names[i];
                listeners = nameListeners[name];

                if (listeners) {
                    for (j = 0,subLn = listeners.length; j < subLn; j++) {
                        listener = listeners[j];
                        listener.fn.call(listener.scope, name);
                    }
                    delete nameListeners[name];
                }
            }
        },

        /**
         * @private
         */
        onCreated: function(fn, scope, className) {
            var listeners = this.createdListeners,
                nameListeners = this.nameCreatedListeners,
                listener = {
                    fn: fn,
                    scope: scope
                };

            if (className) {
                if (this.isCreated(className)) {
                    fn.call(scope, className);
                    return;
                }

                if (!nameListeners[className]) {
                    nameListeners[className] = [];
                }

                nameListeners[className].push(listener);
            }
            else {
                listeners.push(listener);
            }
        },

        /**
         * Supports namespace rewriting
         * @private
         */
        parseNamespace: function(namespace) {
            //<debug error>
            if (typeof namespace != 'string') {
                throw new Error("[Rat.ClassManager] Invalid namespace, must be a string");
            }
            //</debug>

            var cache = this.namespaceParseCache,
                parts,
                rewrites,
                root,
                name,
                rewrite, from, to, i, ln;

            if (this.enableNamespaceParseCache) {
                if (cache.hasOwnProperty(namespace)) {
                    return cache[namespace];
                }
            }

            parts = [];
            rewrites = this.namespaceRewrites;
            root = global;
            name = namespace;

            for (i = 0, ln = rewrites.length; i < ln; i++) {
                rewrite = rewrites[i];
                from = rewrite.from;
                to = rewrite.to;

                if (name === from || name.substring(0, from.length) === from) {
                    name = name.substring(from.length);

                    if (typeof to != 'string') {
                        root = to;
                    } else {
                        parts = parts.concat(to.split('.'));
                    }

                    break;
                }
            }

            parts.push(root);

            parts = parts.concat(name.split('.'));

            if (this.enableNamespaceParseCache) {
                cache[namespace] = parts;
            }

            return parts;
        },

        /**
         * Creates a namespace and assign the `value` to the created object
         *
         *     Rat.ClassManager.setNamespace('MyCompany.pkg.Example', someObject);
         *
         *     alert(MyCompany.pkg.Example === someObject); // alerts true
         *
         * @param {String} name
         * @param {Object} value
         */
        setNamespace: function(name, value) {
            var root = global,
                parts = this.parseNamespace(name),
                ln = parts.length - 1,
                leaf = parts[ln],
                i, part;

            for (i = 0; i < ln; i++) {
                part = parts[i];

                if (typeof part != 'string') {
                    root = part;
                } else {
                    if (!root[part]) {
                        root[part] = {};
                    }

                    root = root[part];
                }
            }

            root[leaf] = value;

            return root[leaf];
        },

        /**
         * The new Rat.ns, supports namespace rewriting
         * @private
         */
        createNamespaces: function() {
            var root = global,
                parts, part, i, j, ln, subLn;

            for (i = 0, ln = arguments.length; i < ln; i++) {
                parts = this.parseNamespace(arguments[i]);

                for (j = 0, subLn = parts.length; j < subLn; j++) {
                    part = parts[j];

                    if (typeof part != 'string') {
                        root = part;
                    } else {
                        if (!root[part]) {
                            root[part] = {};
                        }

                        root = root[part];
                    }
                }
            }

            return root;
        },

        /**
         * Sets a name reference to a class.
         *
         * @param {String} name
         * @param {Object} value
         * @return {Rat.ClassManager} this
         */
        set: function(name, value) {
            var me = this,
                maps = me.maps,
                nameToAlternates = maps.nameToAlternates,
                targetName = me.getName(value),
                alternates;

            me.classes[name] = me.setNamespace(name, value);

            if (targetName && targetName !== name) {
                maps.alternateToName[name] = targetName;
                alternates = nameToAlternates[targetName] || (nameToAlternates[targetName] = []);
                alternates.push(name);
            }

            return this;
        },

        /**
         * Retrieve a class by its name.
         *
         * @param {String} name
         * @return {Rat.Class} class
         */
        get: function(name) {
            var classes = this.classes,
                root,
                parts,
                part, i, ln;

            if (classes[name]) {
                return classes[name];
            }

            root = global;
            parts = this.parseNamespace(name);

            for (i = 0, ln = parts.length; i < ln; i++) {
                part = parts[i];

                if (typeof part != 'string') {
                    root = part;
                } else {
                    if (!root || !root[part]) {
                        return null;
                    }

                    root = root[part];
                }
            }

            return root;
        },

        /**
         * Register the alias for a class.
         *
         * @param {Rat.Class/String} cls a reference to a class or a className
         * @param {String} alias Alias to use when referring to this class
         */
        setAlias: function(cls, alias) {
            var aliasToNameMap = this.maps.aliasToName,
                nameToAliasesMap = this.maps.nameToAliases,
                className;

            if (typeof cls == 'string') {
                className = cls;
            } else {
                className = this.getName(cls);
            }

            if (alias && aliasToNameMap[alias] !== className) {
                //<debug info>
                if (aliasToNameMap[alias] && Rat.isDefined(global.console)) {
                    global.console.log("[Rat.ClassManager] Overriding existing alias: '" + alias + "' " +
                        "of: '" + aliasToNameMap[alias] + "' with: '" + className + "'. Be sure it's intentional.");
                }
                //</debug>

                aliasToNameMap[alias] = className;
            }

            if (!nameToAliasesMap[className]) {
                nameToAliasesMap[className] = [];
            }

            if (alias) {
                Rat.Array.include(nameToAliasesMap[className], alias);
            }

            return this;
        },

        /**
         * Get a reference to the class by its alias.
         *
         * @param {String} alias
         * @return {Rat.Class} class
         */
        getByAlias: function(alias) {
            return this.get(this.getNameByAlias(alias));
        },

        /**
         * Get the name of a class by its alias.
         *
         * @param {String} alias
         * @return {String} className
         */
        getNameByAlias: function(alias) {
            return this.maps.aliasToName[alias] || '';
        },

        /**
         * Get the name of a class by its alternate name.
         *
         * @param {String} alternate
         * @return {String} className
         */
        getNameByAlternate: function(alternate) {
            return this.maps.alternateToName[alternate] || '';
        },

        /**
         * Get the aliases of a class by the class name
         *
         * @param {String} name
         * @return {Array} aliases
         */
        getAliasesByName: function(name) {
            return this.maps.nameToAliases[name] || [];
        },

        /**
         * Get the name of the class by its reference or its instance;
         * usually invoked by the shorthand {@link Rat#getClassName Rat.getClassName}
         *
         *     Rat.ClassManager.getName(Rat.Action); // returns "Rat.Action"
         *
         * @param {Rat.Class/Object} object
         * @return {String} className
         */
        getName: function(object) {
            return object && object.$className || '';
        },

        /**
         * Get the class of the provided object; returns null if it's not an instance
         * of any class created with Rat.define. This is usually invoked by the shorthand {@link Rat#getClass Rat.getClass}
         *
         *     var component = new Rat.Component();
         *
         *     Rat.ClassManager.getClass(component); // returns Rat.Component
         *
         * @param {Object} object
         * @return {Rat.Class} class
         */
        getClass: function(object) {
            return object && object.self || null;
        },

        /**
         * Defines a class.
         * @deprecated 4.1.0 Use {@link Rat#define} instead, as that also supports creating overrides.
         */
        create: function(className, data, createdFn) {
            //<debug error>
            if (typeof className != 'string') {
                throw new Error("[Rat.define] Invalid class name '" + className + "' specified, must be a non-empty string");
            }
            //</debug>

            data.$className = className;

            return new Class(data, function() {
                var postprocessorStack = data.postprocessors || Manager.defaultPostprocessors,
                    registeredPostprocessors = Manager.postprocessors,
                    postprocessors = [],
                    postprocessor, i, ln, j, subLn, postprocessorProperties, postprocessorProperty;

                delete data.postprocessors;

                for (i = 0,ln = postprocessorStack.length; i < ln; i++) {
                    postprocessor = postprocessorStack[i];

                    if (typeof postprocessor == 'string') {
                        postprocessor = registeredPostprocessors[postprocessor];
                        postprocessorProperties = postprocessor.properties;

                        if (postprocessorProperties === true) {
                            postprocessors.push(postprocessor.fn);
                        }
                        else if (postprocessorProperties) {
                            for (j = 0,subLn = postprocessorProperties.length; j < subLn; j++) {
                                postprocessorProperty = postprocessorProperties[j];

                                if (data.hasOwnProperty(postprocessorProperty)) {
                                    postprocessors.push(postprocessor.fn);
                                    break;
                                }
                            }
                        }
                    }
                    else {
                        postprocessors.push(postprocessor);
                    }
                }

                data.postprocessors = postprocessors;
                data.createdFn = createdFn;
                Manager.processCreate(className, this, data);
            });
        },
        
        processCreate: function(className, cls, clsData){
            var me = this,
                postprocessor = clsData.postprocessors.shift(),
                createdFn = clsData.createdFn;

            if (!postprocessor) {
                me.set(className, cls);

                if (createdFn) {
                     createdFn.call(cls, cls);
                }

                me.triggerCreated(className);
                return;
            }

            if (postprocessor.call(me, className, cls, clsData, me.processCreate) !== false) {
                me.processCreate(className, cls, clsData);
            }
        },

        createOverride: function (className, data, createdFn) {
            var me = this,
                overriddenClassName = data.override,
                requires = data.requires,
                uses = data.uses,
                classReady = function () {
                    var cls, temp;

                    if (requires) {
                        temp = requires;
                        requires = null; // do the real thing next time (which may be now)

                        // Since the override is going to be used (its target class is now
                        // created), we need to fetch the required classes for the override
                        // and call us back once they are loaded:
                        Rat.Loader.require(temp, classReady);
                    } else {
                        // The target class and the required classes for this override are
                        // ready, so we can apply the override now:
                        cls = me.get(overriddenClassName);

                        // We don't want to apply these:
                        delete data.override;
                        delete data.requires;
                        delete data.uses;

                        Rat.override(cls, data);

                        // This pushes the overridding file itself into Rat.Loader.history
                        // Hence if the target class never exists, the overriding file will
                        // never be included in the build.
                        me.triggerCreated(className);

                        if (uses) {
                            Rat.Loader.addUsedClasses(uses); // get these classes too!
                        }

                        if (createdFn) {
                            createdFn.call(cls); // last but not least!
                        }
                    }
                };

            me.existCache[className] = true;

            // Override the target class right after it's created
            me.onCreated(classReady, me, overriddenClassName);
 
            return me;
        },

        /**
         * Instantiate a class by its alias; usually invoked by the convenient shorthand {@link Rat#createByAlias Rat.createByAlias}
         * If {@link Rat.Loader} is {@link Rat.Loader#setConfig enabled} and the class has not been defined yet, it will
         * attempt to load the class via synchronous loading.
         *
         *     var window = Rat.ClassManager.instantiateByAlias('widget.window', { width: 600, height: 800, ... });
         *
         * @param {String} alias
         * @param {Object...} args Additional arguments after the alias will be passed to the
         * class constructor.
         * @return {Object} instance
         */
        instantiateByAlias: function() {
            var alias = arguments[0],
                args = arraySlice.call(arguments),
                className = this.getNameByAlias(alias);

            if (!className) {
                className = this.maps.aliasToName[alias];

                //<debug error>
                if (!className) {
                    throw new Error("[Rat.createByAlias] Cannot create an instance of unrecognized alias: " + alias);
                }
                //</debug>

                //<debug warn>
                if (global.console) {
                    global.console.warn("[Rat.Loader] Synchronously loading '" + className + "'; consider adding " +
                         "Rat.require('" + alias + "') above Rat.onReady");
                }
                //</debug>

                Rat.syncRequire(className);
            }

            args[0] = className;

            return this.instantiate.apply(this, args);
        },

        /**
         * @private
         */
        instantiate: function() {
            var name = arguments[0],
                nameType = typeof name,
                args = arraySlice.call(arguments, 1),
                alias = name,
                possibleName, cls;

            if (nameType != 'function') {
                if (nameType != 'string' && args.length === 0) {
                    args = [name];
                    name = name.xclass;
                }

                //<debug error>
                if (typeof name != 'string' || name.length < 1) {
                    throw new Error("[Rat.create] Invalid class name or alias '" + name + "' specified, must be a non-empty string");
                }
                //</debug>

                cls = this.get(name);
            }
            else {
                cls = name;
            }

            // No record of this class name, it's possibly an alias, so look it up
            if (!cls) {
                possibleName = this.getNameByAlias(name);

                if (possibleName) {
                    name = possibleName;

                    cls = this.get(name);
                }
            }

            // Still no record of this class name, it's possibly an alternate name, so look it up
            if (!cls) {
                possibleName = this.getNameByAlternate(name);

                if (possibleName) {
                    name = possibleName;

                    cls = this.get(name);
                }
            }

            // Still not existing at this point, try to load it via synchronous mode as the last resort
            if (!cls) {
                //<debug warn>
                if (global.console) {
                    global.console.warn("[Rat.Loader] Synchronously loading '" + name + "'; consider adding " +
                         "Rat.require('" + ((possibleName) ? alias : name) + "') above Rat.onReady");
                }
                //</debug>

                Rat.syncRequire(name);

                cls = this.get(name);
            }

            //<debug error>
            if (!cls) {
                throw new Error("[Rat.create] Cannot create an instance of unrecognized class name / alias: " + alias);
            }

            if (typeof cls != 'function') {
                throw new Error("[Rat.create] '" + name + "' is a singleton and cannot be instantiated");
            }
            //</debug>

            return this.getInstantiator(args.length)(cls, args);
        },

        /**
         * @private
         * @param name
         * @param args
         */
        dynInstantiate: function(name, args) {
            args = arrayFrom(args, true);
            args.unshift(name);

            return this.instantiate.apply(this, args);
        },

        /**
         * @private
         * @param length
         */
        getInstantiator: function(length) {
            var instantiators = this.instantiators,
                instantiator,
                i,
                args;

            instantiator = instantiators[length];

            if (!instantiator) {
                i = length;
                args = [];

                for (i = 0; i < length; i++) {
                    args.push('a[' + i + ']');
                }

                instantiator = instantiators[length] = new Function('c', 'a', 'return new c(' + args.join(',') + ')');
                //<debug>
                instantiator.displayName = "Rat.ClassManager.instantiate" + length;
                //</debug>
            }

            return instantiator;
        },

        /**
         * @private
         */
        postprocessors: {},

        /**
         * @private
         */
        defaultPostprocessors: [],

        /**
         * Register a post-processor function.
         *
         * @private
         * @param {String} name
         * @param {Function} postprocessor
         */
        registerPostprocessor: function(name, fn, properties, position, relativeTo) {
            if (!position) {
                position = 'last';
            }

            if (!properties) {
                properties = [name];
            }

            this.postprocessors[name] = {
                name: name,
                properties: properties || false,
                fn: fn
            };

            this.setDefaultPostprocessorPosition(name, position, relativeTo);

            return this;
        },

        /**
         * Set the default post processors array stack which are applied to every class.
         *
         * @private
         * @param {String/Array} The name of a registered post processor or an array of registered names.
         * @return {Rat.ClassManager} this
         */
        setDefaultPostprocessors: function(postprocessors) {
            this.defaultPostprocessors = arrayFrom(postprocessors);

            return this;
        },

        /**
         * Insert this post-processor at a specific position in the stack, optionally relative to
         * any existing post-processor
         *
         * @private
         * @param {String} name The post-processor name. Note that it needs to be registered with
         * {@link Rat.ClassManager#registerPostprocessor} before this
         * @param {String} offset The insertion position. Four possible values are:
         * 'first', 'last', or: 'before', 'after' (relative to the name provided in the third argument)
         * @param {String} relativeName
         * @return {Rat.ClassManager} this
         */
        setDefaultPostprocessorPosition: function(name, offset, relativeName) {
            var defaultPostprocessors = this.defaultPostprocessors,
                index;

            if (typeof offset == 'string') {
                if (offset === 'first') {
                    defaultPostprocessors.unshift(name);

                    return this;
                }
                else if (offset === 'last') {
                    defaultPostprocessors.push(name);

                    return this;
                }

                offset = (offset === 'after') ? 1 : -1;
            }

            index = Rat.Array.indexOf(defaultPostprocessors, relativeName);

            if (index !== -1) {
                Rat.Array.splice(defaultPostprocessors, Math.max(0, index + offset), 0, name);
            }

            return this;
        },

        /**
         * Converts a string expression to an array of matching class names. An expression can either refers to class aliases
         * or class names. Expressions support wildcards:
         *
         *      // returns ['Rat.window.Window']
         *     var window = Rat.ClassManager.getNamesByExpression('widget.window');
         *
         *     // returns ['widget.panel', 'widget.window', ...]
         *     var allWidgets = Rat.ClassManager.getNamesByExpression('widget.*');
         *
         *     // returns ['Rat.data.Store', 'Rat.data.ArrayProxy', ...]
         *     var allData = Rat.ClassManager.getNamesByExpression('Rat.data.*');
         *
         * @param {String} expression
         * @return {String[]} classNames
         */
        getNamesByExpression: function(expression) {
            var nameToAliasesMap = this.maps.nameToAliases,
                names = [],
                name, alias, aliases, possibleName, regex, i, ln;

            //<debug error>
            if (typeof expression != 'string' || expression.length < 1) {
                throw new Error("[Rat.ClassManager.getNamesByExpression] Expression " + expression + " is invalid, must be a non-empty string");
            }
            //</debug>

            if (expression.indexOf('*') !== -1) {
                expression = expression.replace(/\*/g, '(.*?)');
                regex = new RegExp('^' + expression + '$');

                for (name in nameToAliasesMap) {
                    if (nameToAliasesMap.hasOwnProperty(name)) {
                        aliases = nameToAliasesMap[name];

                        if (name.search(regex) !== -1) {
                            names.push(name);
                        }
                        else {
                            for (i = 0, ln = aliases.length; i < ln; i++) {
                                alias = aliases[i];

                                if (alias.search(regex) !== -1) {
                                    names.push(name);
                                    break;
                                }
                            }
                        }
                    }
                }

            } else {
                possibleName = this.getNameByAlias(expression);

                if (possibleName) {
                    names.push(possibleName);
                } else {
                    possibleName = this.getNameByAlternate(expression);

                    if (possibleName) {
                        names.push(possibleName);
                    } else {
                        names.push(expression);
                    }
                }
            }

            return names;
        }
    };

    //<feature classSystem.alias>
    /**
     * @cfg {String[]} alias
     * @member Rat.Class
     * List of short aliases for class names.  Most useful for defining xtypes for widgets:
     *
     *     Rat.define('MyApp.CoolPanel', {
     *         extend: 'Rat.panel.Panel',
     *         alias: ['widget.coolpanel'],
     *         title: 'Yeah!'
     *     });
     *
     *     // Using Rat.create
     *     Rat.create('widget.coolpanel');
     *
     *     // Using the shorthand for defining widgets by xtype
     *     Rat.widget('panel', {
     *         items: [
     *             {xtype: 'coolpanel', html: 'Foo'},
     *             {xtype: 'coolpanel', html: 'Bar'}
     *         ]
     *     });
     *
     * Besides "widget" for xtype there are alias namespaces like "feature" for ftype and "plugin" for ptype.
     */
    Manager.registerPostprocessor('alias', function(name, cls, data) {
        var aliases = data.alias,
            i, ln;

        for (i = 0,ln = aliases.length; i < ln; i++) {
            alias = aliases[i];

            this.setAlias(cls, alias);
        }

    }, ['xtype', 'alias']);
    //</feature>

    //<feature classSystem.singleton>
    /**
     * @cfg {Boolean} singleton
     * @member Rat.Class
     * When set to true, the class will be instantiated as singleton.  For example:
     *
     *     Rat.define('Logger', {
     *         singleton: true,
     *         log: function(msg) {
     *             console.log(msg);
     *         }
     *     });
     *
     *     Logger.log('Hello');
     */
    Manager.registerPostprocessor('singleton', function(name, cls, data, fn) {
        fn.call(this, name, new cls(), data);
        return false;
    });
    //</feature>

    //<feature classSystem.alternateClassName>
    /**
     * @cfg {String/String[]} alternateClassName
     * @member Rat.Class
     * Defines alternate names for this class.  For example:
     *
     *     Rat.define('Developer', {
     *         alternateClassName: ['Coder', 'Hacker'],
     *         code: function(msg) {
     *             alert('Typing... ' + msg);
     *         }
     *     });
     *
     *     var joe = Rat.create('Developer');
     *     joe.code('stackoverflow');
     *
     *     var rms = Rat.create('Hacker');
     *     rms.code('hack hack');
     */
    Manager.registerPostprocessor('alternateClassName', function(name, cls, data) {
        var alternates = data.alternateClassName,
            i, ln, alternate;

        if (!(alternates instanceof Array)) {
            alternates = [alternates];
        }

        for (i = 0, ln = alternates.length; i < ln; i++) {
            alternate = alternates[i];

            //<debug error>
            if (typeof alternate != 'string') {
                throw new Error("[Rat.define] Invalid alternate of: '" + alternate + "' for class: '" + name + "'; must be a valid string");
            }
            //</debug>

            this.set(alternate, cls);
        }
    });
    //</feature>

    Rat.apply(Rat, {
        /**
         * Instantiate a class by either full name, alias or alternate name.
         *
         * If {@link Rat.Loader} is {@link Rat.Loader#setConfig enabled} and the class has
         * not been defined yet, it will attempt to load the class via synchronous loading.
         *
         * For example, all these three lines return the same result:
         *
         *      // alias
         *      var window = Rat.create('widget.window', {
         *          width: 600,
         *          height: 800,
         *          ...
         *      });
         *
         *      // alternate name
         *      var window = Rat.create('Rat.Window', {
         *          width: 600,
         *          height: 800,
         *          ...
         *      });
         *
         *      // full class name
         *      var window = Rat.create('Rat.window.Window', {
         *          width: 600,
         *          height: 800,
         *          ...
         *      });
         *
         *      // single object with xclass property:
         *      var window = Rat.create({
         *          xclass: 'Rat.window.Window', // any valid value for 'name' (above)
         *          width: 600,
         *          height: 800,
         *          ...
         *      });
         *
         * @param {String} [name] The class name or alias. Can be specified as `xclass`
         * property if only one object parameter is specified.
         * @param {Object...} [args] Additional arguments after the name will be passed to
         * the class' constructor.
         * @return {Object} instance
         * @member Rat
         * @method create
         */
        create: alias(Manager, 'instantiate'),

        /**
         * Convenient shorthand to create a widget by its xtype or a config object.
         * See also {@link Rat.ClassManager#instantiateByAlias}.
         *
         *      var button = Rat.widget('button'); // Equivalent to Rat.create('widget.button');
         *
         *      var panel = Rat.widget('panel', { // Equivalent to Rat.create('widget.panel')
         *          title: 'Panel'
         *      });
         *
         *      var grid = Rat.widget({
         *          xtype: 'grid',
         *          ...
         *      });
         *
         * If a {@link Rat.Component component} instance is passed, it is simply returned.
         *
         * @member Rat
         * @param {String} [name] The xtype of the widget to create.
         * @param {Object} [config] The configuration object for the widget constructor.
         * @return {Object} The widget instance
         */
        widget: function(name, config) {
            // forms:
            //      1: (xtype)
            //      2: (xtype, config)
            //      3: (config)
            //      4: (xtype, component)
            //      5: (component)
            //      
            var xtype = name,
                alias, className, T, load;

            if (typeof xtype != 'string') { // if (form 3 or 5)
                // first arg is config or component
                config = name; // arguments[0]
                xtype = config.xtype;
            } else {
                config = config || {};
            }
            
            if (config.isComponent) {
                return config;
            }

            alias = 'widget.' + xtype;
            className = Manager.getNameByAlias(alias);

            // this is needed to support demand loading of the class
            if (!className) {
                load = true;
            }
            
            T = Manager.get(className);
            if (load || !T) {
                return Manager.instantiateByAlias(alias, config);
            }
            return new T(config);
        },

        /**
         * Convenient shorthand, see {@link Rat.ClassManager#instantiateByAlias}
         * @member Rat
         * @method createByAlias
         */
        createByAlias: alias(Manager, 'instantiateByAlias'),

        /**
         * @method
         * Defines a class or override. A basic class is defined like this:
         *
         *      Rat.define('My.awesome.Class', {
         *          someProperty: 'something',
         *
         *          someMethod: function() {
         *              alert(s + this.someProperty);
         *          }
         *
         *          ...
         *      });
         *
         *      var obj = new My.awesome.Class();
         *
         *      obj.someMethod('Say '); // alerts 'Say something'
         *
         * To defines an override, include the `override` property. The content of an
         * override is aggregated with the specified class in order to extend or modify
         * that class. This can be as simple as setting default property values or it can
         * extend and/or replace methods. This can also extend the statics of the class.
         *
         * One use for an override is to break a large class into manageable pieces.
         *
         *      // File: /src/app/Panel.js
         *
         *      Rat.define('My.app.Panel', {
         *          extend: 'Rat.panel.Panel',
         *          requires: [
         *              'My.app.PanelPart2',
         *              'My.app.PanelPart3'
         *          ]
         *
         *          constructor: function (config) {
         *              this.callParent(arguments); // calls Rat.panel.Panel's constructor
         *              //...
         *          },
         *
         *          statics: {
         *              method: function () {
         *                  return 'abc';
         *              }
         *          }
         *      });
         *
         *      // File: /src/app/PanelPart2.js
         *      Rat.define('My.app.PanelPart2', {
         *          override: 'My.app.Panel',
         *
         *          constructor: function (config) {
         *              this.callParent(arguments); // calls My.app.Panel's constructor
         *              //...
         *          }
         *      });
         *
         * Another use of overrides is to provide optional parts of classes that can be
         * independently required. In this case, the class may even be unaware of the
         * override altogether.
         *
         *      Rat.define('My.ux.CoolTip', {
         *          override: 'Rat.tip.ToolTip',
         *
         *          constructor: function (config) {
         *              this.callParent(arguments); // calls Rat.tip.ToolTip's constructor
         *              //...
         *          }
         *      });
         *
         * The above override can now be required as normal.
         *
         *      Rat.define('My.app.App', {
         *          requires: [
         *              'My.ux.CoolTip'
         *          ]
         *      });
         *
         * Overrides can also contain statics:
         *
         *      Rat.define('My.app.BarMod', {
         *          override: 'Rat.foo.Bar',
         *
         *          statics: {
         *              method: function (x) {
         *                  return this.callParent([x * 2]); // call Rat.foo.Bar.method
         *              }
         *          }
         *      });
         *
         * IMPORTANT: An override is only included in a build if the class it overrides is
         * required. Otherwise, the override, like the target class, is not included.
         *
         * @param {String} className The class name to create in string dot-namespaced format, for example:
         * 'My.very.awesome.Class', 'FeedViewer.plugin.CoolPager'
         * It is highly recommended to follow this simple convention:
         *  - The root and the class name are 'CamelCased'
         *  - Everything else is lower-cased
         * @param {Object} data The key - value pairs of properties to apply to this class. Property names can be of any valid
         * strings, except those in the reserved listed below:
         *  - `mixins`
         *  - `statics`
         *  - `config`
         *  - `alias`
         *  - `self`
         *  - `singleton`
         *  - `alternateClassName`
         *  - `override`
         *
         * @param {Function} createdFn Optional callback to execute after the class is created, the execution scope of which
         * (`this`) will be the newly created class itself.
         * @return {Rat.Base}
         * @markdown
         * @member Rat
         * @method define
         */
        define: function (className, data, createdFn) {
            if (data.override) {
                return Manager.createOverride.apply(Manager, arguments);
            }

            return Manager.create.apply(Manager, arguments);
        },

        /**
         * Convenient shorthand, see {@link Rat.ClassManager#getName}
         * @member Rat
         * @method getClassName
         */
        getClassName: alias(Manager, 'getName'),

        /**
         * Returns the displayName property or className or object. When all else fails, returns "Anonymous".
         * @param {Object} object
         * @return {String}
         */
        getDisplayName: function(object) {
            if (object) {
                if (object.displayName) {
                    return object.displayName;
                }

                if (object.$name && object.$class) {
                    return Rat.getClassName(object.$class) + '#' + object.$name;
                }

                if (object.$className) {
                    return object.$className;
                }
            }

            return 'Anonymous';
        },

        /**
         * Convenient shorthand, see {@link Rat.ClassManager#getClass}
         * @member Rat
         * @method getClass
         */
        getClass: alias(Manager, 'getClass'),

        /**
         * Creates namespaces to be used for scoping variables and classes so that they are not global.
         * Specifying the last node of a namespace implicitly creates all other nodes. Usage:
         *
         *     Rat.namespace('Company', 'Company.data');
         *
         *     // equivalent and preferable to the above syntax
         *     Rat.ns('Company.data');
         *
         *     Company.Widget = function() { ... };
         *
         *     Company.data.CustomStore = function(config) { ... };
         *
         * @param {String...} namespaces
         * @return {Object} The namespace object.
         * (If multiple arguments are passed, this will be the last namespace created)
         * @member Rat
         * @method namespace
         */
        namespace: alias(Manager, 'createNamespaces')
    });

    /**
     * Old name for {@link Rat#widget}.
     * @deprecated 4.0.0 Use {@link Rat#widget} instead.
     * @method createWidget
     * @member Rat
     */
    Rat.createWidget = Rat.widget;

    /**
     * Convenient alias for {@link Rat#namespace Rat.namespace}.
     * @inheritdoc Rat#namespace
     * @member Rat
     * @method ns
     */
    Rat.ns = Rat.namespace;

    Class.registerPreprocessor('className', function(cls, data) {
        if (data.$className) {
            cls.$className = data.$className;
            //<debug>
            cls.displayName = cls.$className;
            //</debug>
        }
    }, true, 'first');

    Class.registerPreprocessor('alias', function(cls, data) {
        var prototype = cls.prototype,
            xtypes = arrayFrom(data.xtype),
            aliases = arrayFrom(data.alias),
            widgetPrefix = 'widget.',
            widgetPrefixLength = widgetPrefix.length,
            xtypesChain = Array.prototype.slice.call(prototype.xtypesChain || []),
            xtypesMap = Rat.merge({}, prototype.xtypesMap || {}),
            i, ln, alias, xtype;

        for (i = 0,ln = aliases.length; i < ln; i++) {
            alias = aliases[i];

            //<debug error>
            if (typeof alias != 'string' || alias.length < 1) {
                throw new Error("[Rat.define] Invalid alias of: '" + alias + "' for class: '" + name + "'; must be a valid string");
            }
            //</debug>

            if (alias.substring(0, widgetPrefixLength) === widgetPrefix) {
                xtype = alias.substring(widgetPrefixLength);
                Rat.Array.include(xtypes, xtype);
            }
        }

        cls.xtype = data.xtype = xtypes[0];
        data.xtypes = xtypes;

        for (i = 0,ln = xtypes.length; i < ln; i++) {
            xtype = xtypes[i];

            if (!xtypesMap[xtype]) {
                xtypesMap[xtype] = true;
                xtypesChain.push(xtype);
            }
        }

        data.xtypesChain = xtypesChain;
        data.xtypesMap = xtypesMap;

        Rat.Function.interceptAfter(data, 'onClassCreated', function() {
            var mixins = prototype.mixins,
                key, mixin;

            for (key in mixins) {
                if (mixins.hasOwnProperty(key)) {
                    mixin = mixins[key];

                    xtypes = mixin.xtypes;

                    if (xtypes) {
                        for (i = 0,ln = xtypes.length; i < ln; i++) {
                            xtype = xtypes[i];

                            if (!xtypesMap[xtype]) {
                                xtypesMap[xtype] = true;
                                xtypesChain.push(xtype);
                            }
                        }
                    }
                }
            }
        });

        for (i = 0,ln = xtypes.length; i < ln; i++) {
            xtype = xtypes[i];

            //<debug error>
            if (typeof xtype != 'string' || xtype.length < 1) {
                throw new Error("[Rat.define] Invalid xtype of: '" + xtype + "' for class: '" + name + "'; must be a valid non-empty string");
            }
            //</debug>

            Rat.Array.include(aliases, widgetPrefix + xtype);
        }

        data.alias = aliases;

    }, ['xtype', 'alias']);

}(Rat.Class, Rat.Function.alias, Array.prototype.slice, Rat.Array.from, Rat.global));
