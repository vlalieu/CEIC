<?php


/**
 * Expose the kazak field value as a context condition.
 */
class selcntrctx_context_condition_kazak extends context_condition {
  function condition_values() {
    $values = array();
 
    // Get the allowed options from our field, and return these to context
    // as the values for our condition.
    $field = field_info_field('field_kazak');
    $field_values = array('39' => t('Russia'),
'40' => t('kazakhstan'),
'41' => t('Azerbaijan'),
'42' => t('Iran'),
'43' => t('Turkmenistan'));
 
    foreach($field_values as $field_key => $field_value) {
      $values[$field_key] = check_plain($field_value);
    }
    return $values;
//    return array(39,40,41,42,43); // You must then select 1, 2, or 3 checkbox when you add this condition in a context form.

  }
 
  function execute() {
//	drupal_set_message($_SESSION['selected_country']['#value']);
/*
    if(isset($_SESSION['selected_country'])) {
	$aaa = $_SESSION['selected_country']['#value'];
	    foreach ($this->get_contexts($aaa) as $context) {
	      $this->condition_met($context);
    		}
	}
    // Grab the value this node has stored for our field.
*/
//dsm($this);
    if(isset($_SESSION['selected_country']['#value'])) {
        $aaa = $_SESSION['selected_country']['#value'];
 
            foreach ($this->get_contexts($aaa) as $context) {
		//dsm($context);
              $this->condition_met($context,$aaa);
//		context_condition_met($context, $this->plugin);
                }
	}

  }
}

?>
