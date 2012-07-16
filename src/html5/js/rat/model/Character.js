define([], function() {

  /**
   * Character constructor
   */
  var Character = function(position, id) {

    this.id = id;
    this.position = position;

    this.setPosition = function(newPosition) {
      this.position = newPosition;
    },

    /**
     * Moves the character at the given speed.
     * @param {Point} speed The speed in x and y coordinates.
     * @return The updated position.
     */
    this.move = function(speed) {
      this.position.x += speed.x;
      this.position.y += speed.y;
      return this.position;
    }

  };

  return Character;
});
