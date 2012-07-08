define([], function() {
  // TODO fix this image stuff, really ugly to mix model and view
  var Character = function(position, image) {

    var _image = image;
    this.position = position;

    this.updatePosition = function(newPosition) {
      this.position = newPosition;

      _image.style.left = this.position.x + 'px';
      _image.style.top = this.position.y + 'px';
    }

  };

  return Character;
});
