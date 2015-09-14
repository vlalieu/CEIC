(function($) {

jQuery(document).ready(function($) {
  $('#full-width-slider').royalSlider({
    arrowsNav: true,
    loop: false,
    keyboardNavEnabled: true,
    controlsInside: false,
    imageScaleMode: 'fill',
    arrowsNavAutoHide: false,
    autoScaleSlider: true, 
    autoScaleSliderWidth: 1180,     
    autoScaleSliderHeight: 300,
    controlNavigation: 'bullets',
    thumbsFitInViewport: false,
    navigateByClick: true,
    startSlideId: 0,
    transitionType:'fade',
    globalCaption: false,
    autoPlay: {
    		// autoplay options go gere
    		enabled: true,
    		delay: 3000,
    		pauseOnHover: true
    },
    /* size of all images http://help.dimsemenov.com/kb/royalslider-jquery-plugin-faq/adding-width-and-height-properties-to-images */
    imgWidth: 1180,
    imgHeight: 300
  });
});



})(jQuery);



