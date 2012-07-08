define([
  'jquery',
  'core/Class',
  'model/Character'
], function($, Class, Character) {
  var Rat = {
    applyCssStyle: function(image, style) {
      for (var p in style) {
        image.style[p] = style[p];
      }
      image.style.position = "absolute";
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

      var image = new Image();
      image.onload = function() {
      };
      image.src = "http://dl.dropbox.com/u/946335/bina/533/b5684tR.png";
      //document.getElementById('test').appendChild(image);

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
                "style": {
                  "zIndex": 1000,
                },
                "clickable": false,
                "character": true
              },
              "b5684tR": {
                "clickable": true
              }
            },
            "default_locale": "en",
            "key": "publicKey"
          };
     
      $.ajax({
        type: "GET",
        url: baseUrl + chapter + '/' + scene + '.json',
        dataType: 'json',
        success: function(json) {
            fn(tmpManifest);
        },
        error: function(request, status, error) {
          console.log('could not fetch json');
          fn(tmpManifest);
        }
      });
    },

    /**
     * Launch the app at a given chapter and scene.
     * @param {String} chapter The chapter to load.
     * @param {String} scene The scene to load.
     * @param {String} mode Instance of the game. Either 'dom' or 'canvas'.
     */
    launch: function(chapter, scene, mode) {
      var manifest;
      $('#choose').remove();
      $('#viewport').remove();
      $('body').css('background', 'white');
      $('#content-loading').show();
      this.loadScene(chapter, scene, function(json) {
        console.log(json)
        if (mode === 'dom') {
          Rat.loadImages(json.objects, json.widths, Rat.handleLoadDom);
        }
      });
      var sources = {
        yoda: 'http://www.html5canvastutorials.com/demos/assets/yoda.jpg',
        bryan: '/bina/artwork/character3.png',
        bryan1: '/bina/artwork/character1.png'
      };

    },

    launchCanvas: function(objects) {
      var canvas = document.getElementById('canvas');
      var context = canvas.getContext('2d');
      context.canvas.width = window.innerWidth;
      context.canvas.height = window.innerHeight;
      Rat.loadImages(sources, function(images) {
        context.drawImage(images.yoda, 350, 55, 93, 104);
        context.drawImage(images.bryan, 0, 0);
        context.drawImage(images.bryan1, 0, 100);
        $('#content-loading').hide();
        setInterval(function() {
          position.x++;
          context.clearRect(0, 0, canvas.width, canvas.height);
          context.drawImage(images.bryan, position.x, position.y);
          context.drawImage(images.bryan1, position.x * 2, 100);
        }, 50);
      });
    },

    handleLoadDom: function(data, images) {
      var content = document.getElementById('content'),
          dom = document.createElement('div'),
          image, obj;
      
      console.log('on handleLoadDom');
      console.log(data);

      dom.id = 'dom';
      for (var id in images) {
        image = images[id];
        obj = data[id];
        Rat.applyCssStyle(image, obj.style);
        if (obj.character) {
          Rat._character = new Character({x: 0, y: 0}, image);
          image.style.top = Rat._character.x;
          image.style.left = Rat._character.y;
        }
        dom.appendChild(image);
      }
      content.appendChild(dom);
      //window.onclick = Rat.handleClick;
      window.onmousedown = Rat.handleClick;
    },

    handleClick: function(e) {
      console.log('you clicked in the window');
      console.log(arguments);
      Rat._character.updatePosition({x: e.x, y: e.y});
    }
      
  };

  
  Rat.Class = Class;

  return Rat;
});
