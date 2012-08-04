define([
  'jquery',
  'core/Class',
  'model/Director',
  'model/Character'
], function($, Class, Director, Character) {
  var Rat = {
    applyCssStyle: function(image, style) {
      for (var p in style) {
        image.style[p] = style[p];
      }
      image.style.position = "absolute";
    },
    applyClass: function(image, c) {
      if (c) {
        image.className = c;
      }
    },
    /**
     *
     * TODO Fix empty sources problem.
     */
    loadImages: function(sources, widths, fn) {
      var images = {}, loadedImages = 0, numImages = 0,
          baseUrl = 'http://dl.dropbox.com/u/946335/bina/',
          clientWidth = window.innerWidth, width = 0,
          src, i, id, tmp;

      for(id in sources) {
        if (sources.hasOwnProperty(id)) {
          numImages++;
        }
      }

      for (i = widths.length - 1; i > -1; --i) {
        if (clientWidth >= widths[i]) {
          width = widths[i];
          break;
        }
      }

      tmp = document.createElement('p');
      tmp.innerHTML = 'Client: ' + window.innerWidth;
      document.getElementById('test').appendChild(tmp);
      tmp = document.createElement('p');
      tmp.innerHTML = 'Width: ' + width;
      document.getElementById('test').appendChild(tmp);

      for(id in sources) {
        src = sources[id];
        images[id] = new Image();
        images[id].onload = function() {
          if (++loadedImages >= numImages) {
            $("#content-loading").remove();
            // TODO Make a real asset manager bound to the models.
            Rat._images = images;
            fn(sources, images);
          }
        };


        images[id].src = baseUrl + width + '/' + id + '.png';
        tmp = document.createElement('p');
        tmp.innerHTML = images[id].src;
        document.getElementById('test').appendChild(tmp);
      }
    },

    loadScene: function(chapter, scene, fn) {
      var baseUrl = 'http://dl.dropbox.com/u/946335/bina/man/',
          tmpManifest = {
            "name": "Scene 1",
            "manifest_version": "1",
            "version": "0.1",
            "chapter": 1,
            "scene": 1,
            "description": "The first scene",
            "widths": [ 320, 533, 800, 960, 1920 ],
            "objects": {
              "aWET56ET": {
                "class": "character",
                "style": {
                  "zIndex": 1000
                },
                "clickable": false,
                "character": true
              },
              "b5684tR": {
               "class": "background",
                "clickable": true
              }
            },
            "default_locale": "en",
            "key": "publicKey"
          };

      fn(tmpManifest);
      //$.ajax({
        //type: "GET",
        //url: baseUrl + chapter + '/' + scene + '.json',
        //dataType: 'json',
        //success: function(json) {
            //fn(tmpManifest);
        //},
        //error: function(request, status, error) {
          //console.log('could not fetch json');
        //}
      //});
    },

    /**
     * Launch the app at a given chapter and scene.
     * @param {String} chapter The chapter to load.
     * @param {String} scene The scene to load.
     * @param {String} mode Instance of the game. Either 'dom' or 'canvas'.
     */
    launch: function(chapter, scene, mode) {
      /* TODO Make and empty function like in Sencha */
      var cb = function() { };
      $('#choose').remove();
      $('#viewport').remove();
      $('body').css('background', 'white');
      $('#content-loading').show();
      //var image = new Image();
      //image.src = "http://dl.dropbox.com/u/946335/bina/533/aWET56ET.png";
      //document.getElementById('test').appendChild(image);
      this.loadScene(chapter, scene, function(json) {
        console.log("scene loaded with json:");
        console.log(json);
        if (mode === 'dom') {
          cb = Rat.handleLoadDom;
        } else if (mode === 'canvas') {
          cb = Rat.handleLoadCanvas;
        }
        Rat.loadImages(json.objects, json.widths, cb);
      });
    },

    handleLoadDom: function(data, images) {
      var content = document.getElementById('content'),
          dom = document.createElement('div'),
          image, obj;

      dom.id = 'dom';
      for (var id in images) {
        image = images[id];
        obj = data[id];
        Rat.applyCssStyle(image, obj.style);
        Rat.applyClass(image, obj['class']);
        if (obj.character) {
          // TODO Make a real object manager
          Rat._character = new Character({x: 0, y: 0}, id);
        }
        dom.appendChild(image);
      }
      content.appendChild(dom);
      window.setInterval(function() {
        Rat.onRefresh();
      }, 0.016); // 60 fps

      //window.onclick = Rat.handleClick;
      //window.onmousedown = Rat.handleClick;
      //window.ontouchstart = Rat.handleTouchStart;
    },

    handleLoadCanvas: function(data, images) {
      this.director = new Director(100, 200);
    },

    onRefresh: function() {
      var character = Rat._character,
          id = character.id;
      character.move({ x: 1, y: 0});
      if (character.position.x >= window.innerWidth - 50) {
        character.setPosition({ x: 0, y: character.position.y + 100 });
      }
      if (character.position.y >= window.innerHeight - 50) {
        character.setPosition({ x: 0, y: 0 });
      }
      Rat._images[id].style.left = character.position.x + "px";
      Rat._images[id].style.top = character.position.y + "px";
    },

    handleClick: function(e) {
      var tmp;
      tmp = document.createElement('p');
      tmp.innerHTML = 'Click !';
      document.getElementById('test').appendChild(tmp);
      $(".character").animate({
        top: e.y + "px",
        left: e.x + "px"
      }, 1000);
    },

    handleTouchStart: function(e) {
      var tmp, t = e.touches[0];
      $(".character").animate({
        top: t.clientY + "px",
        left: t.clientX + "px"
      }, 1000);
    }

  };

  Rat.Class = Class;

  return Rat;
});
