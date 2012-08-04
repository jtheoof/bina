/**
 * @author: Jeremy Attali <jeremy.attali@gmail.com>
 */

require([
  'js/rat/app.js'
], function (App) {
  var choices = document.getElementsByClassName('choice'),
      i, choice;
  document.getElementById('viewport').innerHTML =
    window.innerWidth + 'x' + window.innerHeight;
  //var image = new Image();
  //image.src = "http://dl.dropbox.com/u/946335/bina/533/b5684tR.png";
  //document.getElementById('test').appendChild(image);
  for (i = 0; i < choices.length; ++i) {
    choice = choices[i];
    choice.onclick = function (e) {
      /* FIXME e.target.dataset does not exist in Galasy S2 GB
       * TODO e.target.dataset does not exist in Galasy S2 GB
       */
      App.initialize(e.target.dataset.instance);
    };
  }
});
