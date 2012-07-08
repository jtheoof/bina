define([
  'jquery',
  'core/Class'
], function($, Class) {
  var Rat = {
    /**
     * Launch the app at a given chapter and scene.
     * @param {String} chapter The chapter to load.
     * @param {String} scene The scene to load.
     */
    launchold: function(chapter, scene) {
      /* Hide loading animation */
      $('#content-loading').hide();
      console.log("loading chapter: " + chapter + " - scene: " + scene);
      $.getJSON('../../man/bina/' + chapter + '/' + scene + '.json',
        function(json) {
          console.log(json);
          console.log(!!window.HTMLCanvasElement);
          var canvas = document.getElementById('canvas'),
              context = canvas.getContext('2d'),
              img1 = new Image(),
              img2 = new Image(),
              img3 = new Image(50, 96);

          console.log(context);
          img1.onload = function() {
            context.drawImage(img1, 0, 0, 178, 343);
            console.log('done');
          };
          img2.onload = function() {
            context.drawImage(img2, 0, 0, 178, 343, 179, 0, 17, 34);
            console.log('done');
          };
          img3.onload = function() {
            context.drawImage(img3, 0, 0);
            console.log('done');
          };
          img1.src = '/bina/artwork/character1.png';
          img2.src = '/bina/artwork/character2.png';
          img3.src = '/bina/artwork/character3.png';
        }
      );
    },
    launch: function(chapter, scene) {
      function loadImages(sources, callback) {
        var images = {};
        var loadedImages = 0;
        var numImages = 0;
        // get num of sources
        for(var src in sources) {
          numImages++;
        }
        for(var src in sources) {
          images[src] = new Image();
          images[src].onload = function() {
            if(++loadedImages >= numImages) {
              callback(images);
            }
          };
          images[src].src = sources[src];
        }
      }

      
        var canvas = document.getElementById("canvas");
        var context = canvas.getContext("2d");
        var position = {
          x: 0,
          y: 0
        };
        context.canvas.width = window.innerWidth;
        context.canvas.height = window.innerHeight;

        var sources = {
          yoda: "http://www.html5canvastutorials.com/demos/assets/yoda.jpg",
          bryan: "/bina/artwork/character3.png",
          bryan1: "/bina/artwork/character1.png"
        };

        loadImages(sources, function(images) {
          context.drawImage(images.yoda, 350, 55, 93, 104);
          context.drawImage(images.bryan, 0, 0);
          context.drawImage(images.bryan1, 0, 100);
          $("#content-loading").hide();
          setInterval(function() {
            position.x++;
            context.clearRect(0, 0, canvas.width, canvas.height);
            context.drawImage(images.bryan, position.x, position.y);
            context.drawImage(images.bryan1, position.x * 2, 100);
          }, 50);
        });

      }
  };
  
  Rat.Class = Class;

  return Rat;
});
