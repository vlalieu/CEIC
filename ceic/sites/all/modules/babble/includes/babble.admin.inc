<?php

/**
 * @file
 * Provides the babble module admin pages.
 */

/**
 * Lists all babble types on a site.
 */
function babble_type_overview() {
  $bundles = field_info_bundles();
  $colspan = module_exists('field_ui') ? '3' : '1';
  $header = array(t('Field name'), t('Used in'), array('data' => t('Operations'), 'colspan' => $colspan));
  $rows = array();

  foreach (field_info_instances() as $entity_type => $type_bundles) {
    foreach ($type_bundles as $bundle => $bundle_instances) {
      foreach ($bundle_instances as $field_name => $instance) {
        $field = field_info_field($field_name);
        if ($field['type'] != 'babble') {
          continue;
        }

        if (!isset($rows[$field_name])) {
          $rows[$field_name]['class'] = $field['locked'] ? array('menu-disabled') : array('');
          $rows[$field_name]['data'][0] = $field['locked'] ? t('@field_name (Locked)', array('@field_name' => $field_name)) : $field_name;
        }

        $rows[$field_name]['data'][1][] = module_exists('field_ui') ? l($bundles[$entity_type][$bundle]['label'], _field_ui_bundle_admin_path($entity_type, $bundle) . '/fields') : $bundles[$entity_type][$bundle]['label'];
      }
    }
  }

  foreach ($rows as $field_name => $cell) {
    $rows[$field_name]['data'][1] = implode(', ', $cell['data'][1]);
    $rows[$field_name]['data'][2] = l(t('edit'), 'admin/structure/babble/' . strtr($field_name, array('_' => '-')) . '/edit');

    if (module_exists('field_ui')) {
      $rows[$field_name]['data'][3] = l(t('manage fields'), 'admin/structure/babble/' . strtr($field_name, array('_' => '-')) . '/fields');
      $rows[$field_name]['data'][4] = l(t('manage display'), 'admin/structure/babble/' . strtr($field_name, array('_' => '-')) . '/display');
    }
  }

  if (empty($rows)) {
    $output = t('No babble types have been defined yet. To do so attach a babble field to any fieldable entity.');
  }

  else {
    // Sort rows by field name.
    ksort($rows);

    $output = theme('table', array('header' => $header, 'rows' => $rows));
  }

  return $output;
}

/**
 * Form constructor for the babble type edit page.
 */
function babble_type_edit_form($form, &$form_state, $field_name) {
  $field = field_info_field($field_name);
  $form_state['field_name'] = $field_name;

  $title_options['_none'] = t('- None -');
  foreach (field_info_instances('babble', $field_name) as $field_name => $instance) {
    $title_options[$field_name] = $instance['label'];
  }

  $form['title_field'] = array(
    '#type' => 'select',
    '#title' => t('Title'),
    '#description' => t("The field from which to retrieve the entity title. Normally, this should be the 'subject' field of the comment (if any)."),
    '#options' => $title_options,
    '#default_value' => isset($field['settings']['title_field']) ? $field['settings']['title_field'] : '_none',
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Form submit handler for the babble type edit page.
 */
function babble_type_edit_form_submit($form, &$form_state) {
  $field = field_info_field($form_state['field_name']);
  $field['settings']['title_field'] = $form_state['values']['title_field'];
  field_update_field($field);
}