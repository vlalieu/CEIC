/**
 * Created with IntelliJ IDEA.
 * User: Sergey Grigorenko
 * Date: 17.07.13
 * Time: 20:14
 */

(function ($) {

    Drupal.behaviors.select2widget = {
        attach: function (context, settings) {

            var version = $.fn.jquery;
            if (version > '1.7') {
                var config = settings.select2widget;
                if (typeof config != 'undefined' && typeof config.elements != 'undefined') {
                    for (var el in config.elements) {
                        var e = $('#' + config.elements[el].id, context);
                        e.select2({ width: '400'});
                    }
                }

                var config = settings.select2widgetNew;
                if (typeof config != 'undefined' && typeof config.elements != 'undefined') {
                    for (var el in config.elements) {
                        var e = $('#' + config.elements[el].id, context);
                        var url = config.elements[el].url;


                        e.select2({
                            width: config.elements[el].width,
                            placeholder: "Search for a movie",
                            minimumInputLength: 1,
                            dropdownAutoWidth: true,
                            ajax: { // instead of writing the function to execute the request we use Select2's convenient helper
                                url: url,
                                data: function (term, page) {
                                    return {
                                        search_string: term // search term
                                    };
                                },
                                dataType: 'json',
                                results: function (data, page) { // parse the results into the format expected by Select2.
                                    // since we are using custom formatting functions we do not need to alter remote JSON data
                                    return {results: data};
                                }
                            },


                            initSelection: function (element, callback) {
                                // the input tag has a value attribute preloaded that points to a preselected movie's id
                                // this function resolves that id attribute to an object that select2 can render
                                // using its formatResult renderer - that way the movie name is shown preselected
                                var id = $(element).val();
                                var data = {title: id};
                                callback({title: id});
                            },
                            formatResult: select2widgetFormatResult, // omitted for brevity, see the source of this page
                            formatSelection: select2widgetFormatSelection,  // omitted for brevity, see the source of this page
                            dropdownCssClass: "bigdrop", // apply css that makes the dropdown taller
                            escapeMarkup: function (m) {
                                return m;
                            }, // we do not want to escape markup since we are displaying html in results
                        });

                    }
                }

            } else {
                alert('Select2 widget required jQuery version >=1.7.1 \nGo to "Configuration -> Development -> jQuery update" and choose "jQuery Version" 1.7 or 1.8.')
            }

        },

        completedCallback: function () {
            // Do nothing. But it's here in case other modules/themes want to override it.
        }


    }

    function select2widgetFormatResult(data) {
        return data.data;
    }


    function select2widgetFormatSelection(data) {
        return data.title;
    }


})(jQuery);


