<?php
/**
 * Created by IntelliJ IDEA.
 * User: svip
 * Date: 19.07.13
 * Time: 17:09
 * To change this template use File | Settings | File Templates.
 */



/**
 * Implements hook_field_widget_settings_form().
 */
function select2widget_field_widget_settings_form($field, $instance) {
  $widget = $instance['widget'];
  $settings = $widget['settings'] + field_info_widget_settings($widget['type']);

  $form = array();

  if ($widget['type'] == 'select2widgetNew'){
    $view_mode_options = array();

    $entity_info = entity_get_info();
    $entity_type = (isset($field['settings']['target_type'])) ? $field['settings']['target_type'] : NULL;

    if (isset($entity_info[$entity_type]['view modes'])) {
      foreach($entity_info[$entity_type]['view modes'] as $key => $value){
        $view_mode_options[$key] = $value['label'];
      }
    }




    $form['select2widget'] = array(
      '#type' => 'fieldset',
      '#title' =>  t('Select2Widget settings'),
      '#tree' => TRUE,
    );


    $form['select2widget']['view_mode'] = array(
      '#type' => 'select',
      '#title' => t('View mode'),
      '#default_value' => (isset($settings['select2widget']['view_mode'])) ? $settings['select2widget']['view_mode'] : reset($view_mode_options),
      '#options' => $view_mode_options,
      '#description' => t('Install !entity_view_mode_link module to create own view modes.',
        array('!entity_view_mode_link' => l(t('"Entity view modes"'), "http://drupal.org/project/entity_view_mode",
        array('attributes' => array('target'=>'_blank'))))),
    );


    $form['select2widget']['match_operator'] = array(
      '#type' => 'select',
      '#title' => t('Search matching'),
      '#default_value' => (isset($settings['select2widget']['match_operator'])) ? $settings['select2widget']['match_operator'] : "",
      '#options' => array(
        'STARTS_WITH' => t('Starts with'),
        'CONTAINS' => t('Contains'),
      ),
      '#description' => t('Select the method used to collect autocomplete suggestions. Note that <em>Contains</em> can cause performance issues on sites with thousands of nodes.'),
    );
    $form['select2widget']['width'] = array(
      '#type' => 'textfield',
      '#size' => '5',
      '#title' => t('Widht of field (px)'),
      '#default_value' => (isset($settings['select2widget']['width'])) ? $settings['select2widget']['width'] : "",
      '#element_validate' => array('_element_validate_integer_positive'),
      '#required' => FALSE,
    );
  }

  return $form;
}


/**
 * Process callback: 'select2widget' element type.
 */
function select2widget_entity_process_callback($element, &$form_state, $form) {
  // Send Drupal.settings a reference to this form element.
  $data['select2widgetNew']['elements'][$element['#id']] = array(
    'id' => $element['#id'],
    'url' => $element['#autocomplete_path'],
    'width' => $element['#widget_width']
  );

  // Attaching library, integration script, and settings array.
  $element['#attached']['library'][] = array('select2widget', 'select2');
  $element['#attached']['js'][] = array(
    'type' => 'setting',
    'data' => $data,
  );

  return $element;
}


/**
 * Return JSON based on given field, instance and string.
 *
 * This function can be used by other modules that wish to pass a mocked
 * definition of the field on instance.
 *
 * @param $type
 *   The widget type (i.e. 'single' or 'tags').
 * @param $field
 *   The field array defintion.
 * @param $instance
 *   The instance array defintion.
 * @param $entity_type
 *   The entity type.
 * @param $entity_id
 *   Optional; The entity ID the entity-reference field is attached to.
 *   Defaults to ''.
 * @param $string
 *   The label of the entity to query by.
 */
function select2widget_callback_get_matches($type, $field, $instance, $entity_type, $entity_id = '', $string = '') {
  $matches = array();

  $entity = NULL;
  if ($entity_id !== 'NULL') {
    $entity = entity_load_single($entity_type, $entity_id);
    if (!$entity || !entity_access('view', $entity_type, $entity)) {
      return MENU_ACCESS_DENIED;
    }
  }

  $handler = entityreference_get_selection_handler($field, $instance, $entity_type, $entity);

  // Get an array of matching entities.
  $entity_labels = $handler->getReferencableEntities($string, $instance['widget']['settings']['select2widget']['match_operator'], 20);
  $matches = select2widget_render_modes($entity_labels, $instance['widget']['settings']['select2widget']['view_mode'], $field['settings']['target_type']);

  drupal_json_output($matches);
}


/**
 * Render entities using display mode
 *
 * @param $entity_labels
 * @param string $view_mode
 * @return array
 */
function select2widget_render_modes($entity_labels, $view_mode = "full", $entity_type = "node") {
  $matches = array();

  foreach ($entity_labels as $values) {
    $ids = array_keys($values);
    $entities = entity_load_multiple_by_name($entity_type, $ids);
    foreach ($entities as $entity) {
      $entity_array = select2widget_render_entity($entity_type, $entity, $view_mode);
      $matches[] = array(
        'id' => $entity_array['id'],
        'title' => $entity_array['title'],
        'data' => '<div class="reference-select2widget">' . render($entity_array['data']) . '</div>',
      );
    }
  }

  return $matches;
}


/**
 * Render single entity
 *
 * @param $entity_type
 * @param $entity
 * @param $view_mode
 * @return array
 */
function select2widget_render_entity($entity_type, $entity, $view_mode = 'full'){
  $entity_array = array('id','title','data');
  switch ($entity_type) {
    case "node":
        $entity_array['id'] = $entity->nid;
        $entity_array['title'] = $entity->title . " - " . $entity->nid;
        $entity_array['data'] = node_view($entity, $view_mode);
      break;
    case "user":
        $entity_array['id'] = $entity->uid;
        $entity_array['title'] = $entity->name . " - " . $entity->uid;
        $entity_array['data'] = user_view($entity, $view_mode);
      break;
  }

  return $entity_array;
}