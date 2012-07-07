(function() {
    Rat.Viewport = {

    };

    Rat.apply(Rat.Viewport, {
        getClientWidth: function() {
            return window.innerWidth;
        },
        getClientHeight: function() {
            return window.innerHeight;
        },
        getClientSize: function() {
            return {
                width: window.innerWidth,
                height: window.innerHeight
            };
        }
    });
}());
