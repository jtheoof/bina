define([
  'core/Rat'
], function(Rat) {
  var initialize = function() {
    console.log('app was initialized');
    Rat.launch("chapter1", "scene1");
  };

  return {
    initialize: initialize
  };
});
