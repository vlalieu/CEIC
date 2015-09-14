( function ($) {
    Drupal.behaviors.countrselector = {
	    // reset all backgrounds to white
        attach: function(context,settings) {
	    $('a[href*="term"]').each( function(i) {
		$(this).css("text-decoration","none").css("color","#7a706e");
		});
		// highlight the selected Tids
		var TidStr = Drupal.settings.tid;
		//alert(TidStr);
		var TidAr=TidStr.split('_');
		for (i=0;i<TidAr.length-1;i++) {
		    var MySelector = 'a[href*="term/' + TidAr[i] + '"]';
		    $(MySelector).css("text-decoration","underline").css("color","black"); // a pepsy green
		}
        }
  };
})(jQuery);
