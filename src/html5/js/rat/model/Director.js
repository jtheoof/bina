define([], function () {

  /**
   * This method performs Director initialization. Must be called once.
   * If the canvas parameter is not set, it will create a Canvas itself,
   * and the developer must explicitly add the canvas to the desired DOM
   * position. This method will also set the Canvas dimension to the specified
   * values by width and height parameters.
   *
   * @param width {Number} A canvas width.
   * @param height {Number} A canvas height.
   * @param canvas {HTMLCanvasElement, String} An optional Canvas object or
   * string to ID.
   * @param proxy {HTMLElement} This object can be an event proxy in case you'd
   * like to layer different elements and want events delivered to the correct
   * element.
   *
   * @return this
   */
  var Director = function (width, height, canvas) {
    var createCanvas = false,
        domCanvas;
    this.width = width;
    this.height = height;
    this.scenes = [];
    this.glEnabled = false; /* toggle experimental-webgl */

    if (canvas) {
      if (typeof canvas === 'string') {
        domCanvas = document.getElementById(canvas);
        if (!domCanvas || domCanvas.nodeName !== 'CANVAS') {
          createCanvas = true;
        } else {
          this.canvas = domCanvas;
        }
      } else if (typeof canvas === 'object') {
        if (canvas.nodeName === 'CANVAS') {
          this.canvas = canvas;
        } else {
          createCanvas = true;
        }
      }
    } else {
      createCanvas = true;
    }
    if (createCanvas) {
      this.canvas = document.createElement('canvas');
      document.body.appendChild(this.canvas);
    }
    this.setBounds(0, 0, width, height);
    return this;
  };

  /**
   * Sets the boundaries of the convas and the scenes assiocated with the
   * director. Also resets WebGL if enabled.
   *
   * @param x {Number} The X coordinate.
   * @param y {Number} The X coordinate.
   * @param w {Number} Width.
   * @param h {Number} Height.
   */
  Director.prototype.setBounds = function (x, y, w, h) {
    var i;
    this.canvas.width = w;
    this.canvas.height = h;
    this.ctx = this.canvas.getContext(
      this.glEnabled ?  'experimental-webgl' : '2d'
    );
    this.crc = this.ctx;

    for (i = 0; i < this.scenes.length; i++) {
      this.scenes[i].setBounds(x, y, w, h);
    }

    if (this.glEnabled) {
      this.glReset();
    }
  };

  Director.prototype.run = function (fps, callback, callback2) {
    return true;
  };

  Director.prototype.glReset = function () {
    throw 'glReset not implemented';
  };

  Director.prototype.requestRepaint = function () {
    throw 'requestRepaint not implemented';
  };
  Director.prototype.getCurrentScene = function () {
    throw 'getCurrentScene not implemented';
  };

  Director.prototype.setRenderMode = function (mode) {
    throw 'setRenderMode not implemented';
  };

  Director.prototype.checkDebug = function () {
    throw 'checkDebug not implemented';
  };

  Director.prototype.getRenderType = function () {
    throw 'getRenderType not implemented';
  };

  Director.prototype.windowResized = function (w, h) {
    throw 'windowResized not implemented';
  };

  Director.prototype.setScaleProportional = function (w, h) {
    throw 'setScaleProportional not implemented';
  };

  Director.prototype.enableResizeEvents = function (mode, onResizeCallback) {
    throw 'enableResizeEvents not implemented';
  };

  Director.prototype.initializeGL = function (width, height, canvas, proxy) {
    throw 'initializeGL not implemented';
  };

  Director.prototype.createScene = function () {
    throw 'createScene not implemented';
  };

  Director.prototype.setImagesCache = function (imagesCache, tpW, tpH) {
    throw 'setImagesCache not implemented';
  };

  Director.prototype.updateGLPages = function () {
    throw 'updateGLPages not implemented';
  };

  Director.prototype.setGLTexturePage = function (tp) {
    throw 'setGLTexturePage not implemented';
  };

  Director.prototype.addImage = function (id, image, noUpdateGL) {
    throw 'addImage not implemented';
  };

  Director.prototype.deleteImage = function (id, noUpdateGL) {
    throw 'deleteImage not implemented';
  };

  Director.prototype.setGLCurrentOpacity = function (opacity) {
    throw 'setGLCurrentOpacity not implemented';
  };

  Director.prototype.glRender = function (vertex, coordsIndex, uv) {
    throw 'glRender not implemented';
  };

  Director.prototype.glFlush = function () {
    throw 'glFlush not implemented';
  };

  Director.prototype.findActorAtPosition = function (point) {
    throw 'findActorAtPosition not implemented';
  };

  Director.prototype.resetStats = function () {
    throw 'resetStats not implemented';
  };

  Director.prototype.render = function (time) {
    throw 'render not implemented';
  };

  Director.prototype.animate = function (director, time) {
    throw 'animate not implemented';
  };

  Director.prototype.addDirtyRect = function (rectangle) {
    throw 'addDirtyRect not implemented';
  };

  Director.prototype.renderToContext = function (ctx, scene) {
    throw 'renderToContext not implemented';
  };

  Director.prototype.addScene = function (scene) {
    throw 'addScene not implemented';
  };

  Director.prototype.getNumScenes = function () {
    throw 'getNumScenes not implemented';
  };

  Director.prototype.easeInOut = function (inSceneIndex, typein, anchorin,
      outSceneIndex, typeout, anchorout, time, alpha, interpolatorIn,
      interpolatorOut) {
    throw 'easeInOut not implemented';
  };

  Director.prototype.easeInOutRandom = function (inIndex, outIndex, time,
      alpha) {
    throw 'easeInOutRandom not implemented';
  };

  Director.prototype.easeIn = function (inSceneIndex, type, time, alpha,
      anchor, interpolator) {
    throw 'easeIn not implemented';
  };

  Director.prototype.setScene = function (sceneIndex) {
    throw 'setScene not implemented';
  };

  Director.prototype.switchToScene = function (iNewSceneIndex, time,
      alpha, transition) {
    throw 'switchToScene not implemented';
  };

  Director.prototype.switchToPrevScene = function (time, alpha, transition) {
    throw 'switchToPrevScene not implemented';
  };

  Director.prototype.switchToNextScene = function (time, alpha, transition) {
    throw 'switchToNextScene= not implemented';
  };

  Director.prototype.mouseEnter = function (mouseEvent) {
    throw 'mouseEnter not implemented';
  };

  Director.prototype.mouseExit = function (mouseEvent) {
    throw 'mouseExit not implemented';
  };

  Director.prototype.mouseMove = function (mouseEvent) {
    throw 'mouseMove not implemented';
  };

  Director.prototype.mouseDown = function (mouseEvent) {
    throw 'mouseDown not implemented';
  };

  Director.prototype.mouseUp = function (mouseEvent) {
    throw 'mouseUp not implemented';
  };

  Director.prototype.mouseDrag = function (mouseEvent) {
    throw 'mouseDrag not implemented';
  };

  Director.prototype.easeEnd = function (scene, b_easeIn) {
    throw 'easeEnd not implemented';
  };

  Director.prototype.getSceneIndex = function (scene) {
    throw 'getSceneIndex not implemented';
  };

  Director.prototype.getScene = function (index) {
    throw 'getScene not implemented';
  };

  Director.prototype.getCurrentSceneIndex = function () {
    throw 'getCurrentSceneIndex not implemented';
  };

  Director.prototype.getBrowserName = function () {
    throw 'getBrowserName not implemented';
  };

  Director.prototype.getBrowserVersion = function () {
    throw 'getBrowserVersion not implemented';
  };

  Director.prototype.getOSName = function () {
    throw 'getOSName not implemented';
  };

  Director.prototype.getImage = function (sId) {
    throw 'getImage not implemented';
  };

  Director.prototype.addAudio = function (id, url) {
    throw 'addAudio not implemented';
  };

  Director.prototype.audioPlay = function (id) {
    throw 'audioPlay not implemented';
  };

  Director.prototype.audioLoop = function (id) {
    throw 'audioLoop not implemented';
  };

  Director.prototype.endSound = function () {
    throw 'endSound not implemented';
  };

  Director.prototype.setSoundEffectsEnabled = function (enabled) {
    throw 'setSoundEffectsEnabled not implemented';
  };

  Director.prototype.setMusicEnabled = function (enabled) {
    throw 'setMusicEnabled not implemented';
  };

  Director.prototype.isMusicEnabled = function () {
    throw 'isMusicEnabled not implemented';
  };

  Director.prototype.isSoundEffectsEnabled = function () {
    throw 'isSoundEffectsEnabled not implemented';
  };

  Director.prototype.setVolume = function (id, volume) {
    throw 'setVolume not implemented';
  };

  Director.prototype.emptyScenes = function () {
    throw 'emptyScenes not implemented';
  };

  Director.prototype.addChild = function (scene) {
    throw 'addChild not implemented';
  };

  Director.prototype.renderFrame = function () {
    throw 'renderFrame not implemented';
  };

  Director.prototype.resetTimeline = function () {
    throw 'resetTimeline not implemented';
  };

  Director.prototype.endLoop = function () {
    throw 'endLoop not implemented';
  };

  Director.prototype.setClear = function (clear) {
    throw 'setClear not implemented';
  };

  Director.prototype.getAudioManager = function () {
    throw 'getAudioManager not implemented';
  };

  Director.prototype.cumulateOffset = function (node, parent, prop) {
    throw 'cumulateOffset not implemented';
  };

  Director.prototype.getOffset = function (node) {
    throw 'getOffset not implemented';
  };

  Director.prototype.getCanvasCoord = function (point, e) {
    throw 'getCanvasCoord not implemented';
  };

  Director.prototype.__mouseDownHandler = function (e) {
    throw '__mouseDownHandler not implemented';
  };

  Director.prototype.__mouseUpHandler = function (e) {
    throw '__mouseUpHandler not implemented';
  };

  Director.prototype.__mouseMoveHandler = function (e) {
    throw '__mouseMoveHandler not implemented';
  };

  Director.prototype.__mouseOutHandler = function (e) {
    throw '__mouseOutHandler not implemented';
  };

  Director.prototype.__mouseOverHandler = function (e) {
    throw '__mouseOverHandler not implemented';
  };

  Director.prototype.__mouseDBLClickHandler = function (e) {
    throw '__mouseDBLClickHandler not implemented';
  };

  Director.prototype.__touchStartHandler = function (e) {
    throw '__touchStartHandler not implemented';
  };

  Director.prototype.__touchEndHandler = function (e) {
    throw '__touchEndHandler not implemented';
  };

  Director.prototype.__touchMoveHandler = function (e) {
    throw '__touchMoveHandler not implemented';
  };

  Director.prototype.__gestureStart = function (scale, rotation) {
    throw '__gestureStart not implemented';
  };

  Director.prototype.__gestureChange = function (scale, rotation) {
    throw '__gestureChange not implemented';
  };

  Director.prototype.__gestureEnd = function (scale, rotation) {
    throw '__gestureEnd not implemented';
  };

  Director.prototype.__touchEndHandlerMT = function (e) {
    throw '__touchEndHandlerMT not implemented';
  };

  Director.prototype.__touchMoveHandlerMT = function (e) {
    throw '__touchMoveHandlerMT not implemented';
  };

  Director.prototype.__touchCancelHandleMT = function (e) {
    throw '__touchCancelHandleMT not implemented';
  };

  Director.prototype.__touchStartHandlerMT = function (e) {
    throw '__touchStartHandlerMT not implemented';
  };

  Director.prototype.__findTouchFirstActor = function () {
    throw '__findTouchFirstActor not implemented';
  };

  Director.prototype.__touchGestureStartHandleMT = function (e) {
    throw '__touchGestureStartHandleMT not implemented';
  };

  Director.prototype.__touchGestureEndHandleMT = function (e) {
    throw '__touchGestureEndHandleMT not implemented';
  };

  Director.prototype.__touchGestureChangeHandleMT = function (e) {
    throw '__touchGestureChangeHandleMT not implemented';
  };

  Director.prototype.addHandlers = function (canvas) {
    throw 'addHandlers= not implemented';
  };

  Director.prototype.enableEvents = function (onElement) {
    throw 'enableEvents not implemented';
  };

  Director.prototype.createEventHandler = function (onElement) {
    throw 'createEventHandler not implemented';
  };

  return Director;
});
