define([
  'core/Rat'
], function(Rat) {
  var initialize = function(mode) {
    console.log('app was ininitialized with ' + mode);
    Rat.launch('chapter1', 'scene1', mode);
  };

  return {
    initialize: initialize
  };
});
