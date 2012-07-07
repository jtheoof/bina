var Rat = Rat || {};
Rat._startTime = new Date().getTime();

(function() {
    var global = this,
        objectPrototype = Object.prototype,
        toString = objectPrototype.toString,
        enumerables = false,
        emptyFn = function () {},
        // This is the "$previous" method of a hook function on an instance. When called, it
        // calls through the class prototype by the name of the called method.
        callOverrideParent = function () {
            var method = callOverrideParent.caller.caller; // skip callParent (our caller)
            return method.$owner.prototype[method.$name].apply(this, arguments);
        },
        i;

    Rat.global = global;

    /**
     * Copies all the properties of config to the specified object.
     * Note that if recursive merging and cloning without referencing the original objects / arrays is needed, use
     * {@link Rat.Object#merge} instead.
     * @param {Object} object The receiver of the properties
     * @param {Object} config The source of the properties
     * @param {Object} [defaults] A different object that will also be applied for default values
     * @return {Object} returns obj
     */
    Rat.apply = function(object, config, defaults) {
        if (defaults) {
            Rat.apply(object, defaults);
        }

        if (object && config && typeof config === 'object') {
            var i, j, k;

            for (i in config) {
                object[i] = config[i];
            }

            if (enumerables) {
                for (j = enumerables.length; j--;) {
                    k = enumerables[j];
                    if (config.hasOwnProperty(k)) {
                        object[k] = config[k];
                    }
                }
            }
        }

        return object;
    };
    
    Rat.apply(Rat, {
        extend: function() {
            console.log("extending");
        },
        inherit: function() {
            console.log("inheriting");
        },
        empty: Rat.emptyFn
    });

    // A full set of static methods to do type checking
    Rat.apply(Rat, {

        /**
         * Returns the given value itself if it's not empty, as described in {@link Rat#isEmpty}; returns the default
         * value (second argument) otherwise.
         *
         * @param {Object} value The value to test
         * @param {Object} defaultValue The value to return if the original value is empty
         * @param {Boolean} allowBlank (optional) true to allow zero length strings to qualify as non-empty (defaults to false)
         * @return {Object} value, if non-empty, else defaultValue
         */
        valueFrom: function(value, defaultValue, allowBlank){
            return Rat.isEmpty(value, allowBlank) ? defaultValue : value;
        },

        /**
         * Returns the type of the given variable in string format. List of possible values are:
         *
         * - `undefined`: If the given value is `undefined`
         * - `null`: If the given value is `null`
         * - `string`: If the given value is a string
         * - `number`: If the given value is a number
         * - `boolean`: If the given value is a boolean value
         * - `date`: If the given value is a `Date` object
         * - `function`: If the given value is a function reference
         * - `object`: If the given value is an object
         * - `array`: If the given value is an array
         * - `regexp`: If the given value is a regular expression
         * - `element`: If the given value is a DOM Element
         * - `textnode`: If the given value is a DOM text node and contains something other than whitespace
         * - `whitespace`: If the given value is a DOM text node and contains only whitespace
         *
         * @param {Object} value
         * @return {String}
         * @markdown
         */
        typeOf: function(value) {
            var type,
                typeToString;
            
            if (value === null) {
                return 'null';
            }

            type = typeof value;

            if (type === 'undefined' || type === 'string' || type === 'number' || type === 'boolean') {
                return type;
            }

            typeToString = toString.call(value);

            switch(typeToString) {
                case '[object Array]':
                    return 'array';
                case '[object Date]':
                    return 'date';
                case '[object Boolean]':
                    return 'boolean';
                case '[object Number]':
                    return 'number';
                case '[object RegExp]':
                    return 'regexp';
            }

            if (type === 'function') {
                return 'function';
            }

            if (type === 'object') {
                if (value.nodeType !== undefined) {
                    if (value.nodeType === 3) {
                        return (/\S/).test(value.nodeValue) ? 'textnode' : 'whitespace';
                    }
                    else {
                        return 'element';
                    }
                }

                return 'object';
            }

        },

        /**
         * Returns true if the passed value is empty, false otherwise. The value is deemed to be empty if it is either:
         *
         * - `null`
         * - `undefined`
         * - a zero-length array
         * - a zero-length string (Unless the `allowEmptyString` parameter is set to `true`)
         *
         * @param {Object} value The value to test
         * @param {Boolean} allowEmptyString (optional) true to allow empty strings (defaults to false)
         * @return {Boolean}
         * @markdown
         */
        isEmpty: function(value, allowEmptyString) {
            return (value === null) || (value === undefined) || (!allowEmptyString ? value === '' : false) || (Rat.isArray(value) && value.length === 0);
        },

        /**
         * Returns true if the passed value is a JavaScript Array, false otherwise.
         *
         * @param {Object} target The target to test
         * @return {Boolean}
         * @method
         */
        isArray: ('isArray' in Array) ? Array.isArray : function(value) {
            return toString.call(value) === '[object Array]';
        },

        /**
         * Returns true if the passed value is a JavaScript Date object, false otherwise.
         * @param {Object} object The object to test
         * @return {Boolean}
         */
        isDate: function(value) {
            return toString.call(value) === '[object Date]';
        },

        /**
         * Returns true if the passed value is a JavaScript Object, false otherwise.
         * @param {Object} value The value to test
         * @return {Boolean}
         * @method
         */
        isObject: (toString.call(null) === '[object Object]') ?
        function(value) {
            // check ownerDocument here as well to exclude DOM nodes
            return value !== null && value !== undefined && toString.call(value) === '[object Object]' && value.ownerDocument === undefined;
        } :
        function(value) {
            return toString.call(value) === '[object Object]';
        },

        /**
         * @private
         */
        isSimpleObject: function(value) {
            return value instanceof Object && value.constructor === Object;
        },

        /**
         * Returns true if the passed value is a JavaScript 'primitive', a string, number or boolean.
         * @param {Object} value The value to test
         * @return {Boolean}
         */
        isPrimitive: function(value) {
            var type = typeof value;

            return type === 'string' || type === 'number' || type === 'boolean';
        },

        /**
         * Returns true if the passed value is a JavaScript Function, false otherwise.
         * @param {Object} value The value to test
         * @return {Boolean}
         * @method
         */
        isFunction:
        // Safari 3.x and 4.x returns 'function' for typeof <NodeList>, hence we need to fall back to using
        // Object.prototype.toString (slower)
        (typeof document !== 'undefined' && typeof document.getElementsByTagName('body') === 'function') ? function(value) {
            return toString.call(value) === '[object Function]';
        } : function(value) {
            return typeof value === 'function';
        },

        /**
         * Returns true if the passed value is a number. Returns false for non-finite numbers.
         * @param {Object} value The value to test
         * @return {Boolean}
         */
        isNumber: function(value) {
            return typeof value === 'number' && isFinite(value);
        },

        /**
         * Validates that a value is numeric.
         * @param {Object} value Examples: 1, '1', '2.34'
         * @return {Boolean} True if numeric, false otherwise
         */
        isNumeric: function(value) {
            return !isNaN(parseFloat(value)) && isFinite(value);
        },

        /**
         * Returns true if the passed value is a string.
         * @param {Object} value The value to test
         * @return {Boolean}
         */
        isString: function(value) {
            return typeof value === 'string';
        },

        /**
         * Returns true if the passed value is a boolean.
         *
         * @param {Object} value The value to test
         * @return {Boolean}
         */
        isBoolean: function(value) {
            return typeof value === 'boolean';
        },

        /**
         * Returns true if the passed value is an HTMLElement
         * @param {Object} value The value to test
         * @return {Boolean}
         */
        isElement: function(value) {
            return value ? value.nodeType === 1 : false;
        },

        /**
         * Returns true if the passed value is a TextNode
         * @param {Object} value The value to test
         * @return {Boolean}
         */
        isTextNode: function(value) {
            return value ? value.nodeName === "#text" : false;
        },

        /**
         * Returns true if the passed value is defined.
         * @param {Object} value The value to test
         * @return {Boolean}
         */
        isDefined: function(value) {
            return typeof value !== 'undefined';
        },
    });

}())

Rat.Function = {

    /**
     * A very commonly used method throughout the framework. It acts as a wrapper around another method
     * which originally accepts 2 arguments for `name` and `value`.
     * The wrapped function then allows "flexible" value setting of either:
     *
     * - `name` and `value` as 2 arguments
     * - one single object argument with multiple key - value pairs
     *
     * For example:
     *
     *     var setValue = Ext.Function.flexSetter(function(name, value) {
     *         this[name] = value;
     *     });
     *
     *     // Afterwards
     *     // Setting a single name - value
     *     setValue('name1', 'value1');
     *
     *     // Settings multiple name - value pairs
     *     setValue({
     *         name1: 'value1',
     *         name2: 'value2',
     *         name3: 'value3'
     *     });
     *
     * @param {Function} setter
     * @returns {Function} flexSetter
     */
    flexSetter: function(fn) {
        return function(a, b) {
            var k, i;

            if (a === null) {
                return this;
            }

            if (typeof a !== 'string') {
                for (k in a) {
                    if (a.hasOwnProperty(k)) {
                        fn.call(this, k, a[k]);
                    }
                }

                if (Ext.enumerables) {
                    for (i = Ext.enumerables.length; i--;) {
                        k = Ext.enumerables[i];
                        if (a.hasOwnProperty(k)) {
                            fn.call(this, k, a[k]);
                        }
                    }
                }
            } else {
                fn.call(this, a, b);
            }

            return this;
        };
    },

    /**
     * Create an alias to the provided method property with name `methodName` of `object`.
     * Note that the execution scope will still be bound to the provided `object` itself.
     *
     * @param {Object/Function} object
     * @param {String} methodName
     * @return {Function} aliasFn
     */
    alias: function(object, methodName) {
        return function() {
            return object[methodName].apply(object, arguments);
        };
    },
};

Rat.Array = {
  
    /**
     * Converts any iterable (numeric indices and a length property) into a true array.
     *
     *     function test() {
     *         var args = Ext.Array.toArray(arguments),
     *             fromSecondToLastArgs = Ext.Array.toArray(arguments, 1);
     *
     *         alert(args.join(' '));
     *         alert(fromSecondToLastArgs.join(' '));
     *     }
     *
     *     test('just', 'testing', 'here'); // alerts 'just testing here';
     *                                      // alerts 'testing here';
     *
     *     Ext.Array.toArray(document.getElementsByTagName('div')); // will convert the NodeList into an array
     *     Ext.Array.toArray('splitted'); // returns ['s', 'p', 'l', 'i', 't', 't', 'e', 'd']
     *     Ext.Array.toArray('splitted', 0, 3); // returns ['s', 'p', 'l']
     *
     * {@link Ext#toArray Ext.toArray} is alias for {@link Ext.Array#toArray Ext.Array.toArray}
     *
     * @param {Object} iterable the iterable object to be turned into a true Array.
     * @param {Number} start (Optional) a zero-based index that specifies the start of extraction. Defaults to 0
     * @param {Number} end (Optional) a 1-based index that specifies the end of extraction. Defaults to the last
     * index of the iterable value
     * @return {Array} array
     */
    toArray: function(iterable, start, end){
        if (!iterable || !iterable.length) {
            return [];
        }

        if (typeof iterable === 'string') {
            iterable = iterable.split('');
        }

        if (supportsSliceOnNodeList) {
            return slice.call(iterable, start || 0, end || iterable.length);
        }

        var array = [],
            i;

        start = start || 0;
        end = end ? ((end < 0) ? iterable.length + end : end) : iterable.length;

        for (i = start; i < end; i++) {
            array.push(iterable[i]);
        }

        return array;
    },

    /**
     * Converts a value to an array if it's not already an array; returns:
     *
     * - An empty array if given value is `undefined` or `null`
     * - Itself if given value is already an array
     * - An array copy if given value is {@link Rat#isIterable iterable} (arguments, NodeList and alike)
     * - An array with one item which is the given value, otherwise
     *
     * @param {Object} value The value to convert to an array if it's not already is an array
     * @param {Boolean} newReference (Optional) True to clone the given array and return a new reference if necessary,
     * defaults to false
     * @return {Array} array
     */
    from: function(value, newReference) {
        if (value === undefined || value === null) {
            return [];
        }

        if (Rat.isArray(value)) {
            return (newReference) ? slice.call(value) : value;
        }

        var type = typeof value;
        // Both strings and functions will have a length property. In phantomJS, NodeList
        // instances report typeof=='function' but don't have an apply method...
        if (value && value.length !== undefined && type !== 'string' && (type !== 'function' || !value.apply)) {
            return Rat.Array.toArray(value);
        }

        return [value];
    },
};

