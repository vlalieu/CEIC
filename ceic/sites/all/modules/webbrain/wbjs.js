( function ($) {
    Drupal.behaviors.webbrain = {
	    // reset all backgrounds to white
        attach: function(context,settings) {
		// two variants - source:drupal.org/node/2046693
	    var pathStr = "get/ajax/node/" + Drupal.settings.thought;
            alert('on passe par' + pathStr);
        //var pathStr = "node/get/ajax/" + Drupal.settings.nid; // variante avec load(node)
        $("#idee_note").load(pathStr);
        }
  };
})(jQuery);
