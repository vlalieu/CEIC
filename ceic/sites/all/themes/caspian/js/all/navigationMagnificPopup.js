(function($) {
	$(document).ready(function() {
        $('.popup').magnificPopup({
          type: 'ajax',
          alignTop: true,
          overflowY: 'scroll' // as we know that popup content is tall we set scroll overflow by default to avoid jump
	});
});
})(jQuery);