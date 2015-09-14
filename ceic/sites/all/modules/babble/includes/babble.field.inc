<?php

/**
 * Implements hook_field_info().
 */
function babble_field_info() {
  $info['babble'] = array(
    'label' => t('Babble'),
    'description' => t('Enables users to comment on entities of the entity type that this field is attached to.'),
    'default_widget' => 'babble',
    'default_formatter' => 'babble_default',
    'settings' => array(
      'title_field' => BABBLE_TITLE_FIELD_UNDEFINED,
    ),
    'instance_settings' => array(
      'access' => BABBLE_COMMENTS_CLOSED,
    ),
  );

  return $info;
}

/**
 * Implements hook_field_extra_fields().
 */
function babble_field_extra_fields() {
  $extra = array();

  foreach (field_info_fields() as $field_name => $field) {
    if ($field['type'] != 'babble') {
      continue;
    }

    $extra['babble'][$field_name]['display']['babble_comment_links'] = array(
      'label' => t('Comment links'),
      'description' => t('Babble module element.'),
      'weight' => 0,
    );

    $extra['babble'][$field_name]['display']['babble_comment_author'] = array(
      'label' => t('Comment author'),
      'description' => t('Babble module element.'),
      'weight' => 0,
    );

    foreach ($field['bundles'] as $entity_type => $bundles) {
      foreach ($bundles as $bundle) {
        $instance = field_info_instance($entity_type, $field_name, $bundle);

        $extra[$entity_type][$bundle]['display'][$field_name . '_babble_comment_form'] = array(
          'label' => t('Comment form (@instance)', array('@instance' => $instance['label'])),
          'description' => t('Babble module element.'),
          'weight' => 0,
        );

        $extra[$entity_type][$bundle]['display'][$field_name . '_babble_host_links'] = array(
          'label' => t('Comment links (@instance)', array('@instance' => $instance['label'])),
          'description' => t('Babble module element.'),
          'weight' => 0,
        );
      }
    }
  }

  return $extra;
}

/**
 * Implements hook_field_formatter_info().
 */
function babble_field_formatter_info() {
  $info['babble_default'] = array(
    'label' => t('Default'),
    'field types' => array('babble'),
  );

  return $info;
}

/**
 * Implements_field_widget_info
 */
function babble_field_widget_info() {
  $info['babble'] = array(
    'label' => t('Babble'),
    'field types' => array('babble'),
    'behaviors' => array(
      'multiple values' => FIELD_BEHAVIOR_DEFAULT,
      'default value' => FIELD_BEHAVIOR_NONE,
    ),
  );

  return $info;
}

/**
 * Implements hook_field_is_empty().
 */
function babble_field_is_empty($item, $field) {
  return FALSE;
}

/**
 * Implements hook_field_access().
 */
function babble_field_access($op, $field, $entity_type, $entity, $account) {
  if ($field['type'] == 'babble') {
    if ($op == 'edit') {
      // Users that do not have the 'manage comment access' permission are not
      // allowed to configure whether an entity is open, closed or hidden for
      // comments.
      return user_access('manage babble access', $account);
    }
    elseif ($op == 'view') {
      return babble_access('view', NULL, $account, $field['field_name'], $entity_type, $entity);
    }
  }
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function babble_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {

}

/**
 * Implements hook_field_formatter_view().
 */
function babble_field_prepare_view($entity_type, $entities, $field, $instances, $langcode, &$items) {
  foreach ($entities as $id => $entity) {
    if (empty($items[$id])) {
      // Make sure that the field is never empty so we can render the comments
      // through the formatter.
      $items[$id][0] = array(
        'access' => BABBLE_COMMENTS_CLOSED,
      );
    }
  }
}

/**
 * Implements hook_field_formatter_view().
 */
function babble_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $elements = array();

  if (babble_access('view', NULL, NULL, $field['field_name'], $entity_type, $entity)) {
    switch ($display['type']) {
      case 'babble_default':
        list($entity_id) = entity_extract_ids($entity_type, $entity);

        if ($entities = babble_load_by_host($entity_type, $entity_id, $field['field_name'])) {
          $output = babble_view($entities, 'list');
          $elements[0] = array('#markup' => drupal_render($output));
        }

        break;
    }
  }

  return $elements;
}

/**
 * Implements hook_field_attach_view_alter().
 */
function babble_field_attach_view_alter(&$output, $context) {
  list(, , $bundle) = entity_extract_ids($context['entity_type'], $context['entity']);

  if ($context['entity_type'] == 'babble') {
    $output['babble_comment_links'] = array(
      '#theme' => 'links__babble_comment_links',
      '#links' => babble_comment_links($context['entity']),
      '#attributes' => array(
        'class' => array('links', 'inline', 'babble-comment-links'),
      ),
    );

    // Fix for rdf_field_attach_view_alter() as described at
    // http://drupal.org/node/
    $output['babble_comment_links']['#field_type'] = FALSE;
  }

  foreach (field_info_fields() as $field_name => $field) {
    if ($field['type'] != 'babble' || !array_key_exists($context['entity_type'], $field['bundles']) || !in_array($bundle, $field['bundles'][$context['entity_type']])) {
      continue;
    }

    if (babble_access('create', NULL, NULL, $field_name, $context['entity_type'], $context['entity'])) {
      // Backup the current page title because entity_ui_get_form() overrides it.
      $title = drupal_get_title();

      // Add the comment form to the output.
      $babble = entity_create('babble', array());
      $babble->setHost($context['entity_type'], $context['entity'], $field_name);

      $output[$field_name . '_babble_comment_form'] = drupal_get_form('babble_form', $babble);
      $output[$field_name . '_babble_comment_form']['#prefix'] = '<h2 class="title">' . t('Add new comment') . '</h2>';

        // Fix for rdf_field_attach_view_alter() as described at
      // http://drupal.org/node/
      $output[$field_name . '_babble_comment_form']['#field_type'] = FALSE;

      // Restore the previous page title.
      drupal_set_title($title, PASS_THROUGH);
    }

    // Add the comment links to the output.
    $output[$field_name . '_babble_host_links'] = array(
      '#theme' => 'links__babble_host_links',
      '#links' => babble_host_links($context['entity_type'], $context['entity'], $field_name),
      '#attributes' => array(
        'class' => array('links', 'inline', 'babble-host-links'),
      ),
    );

    // Fix for rdf_field_attach_view_alter() as described at
    // http://drupal.org/node/.
    $output[$field_name . '_babble_host_links']['#field_type'] = FALSE;
  }
}

/**
 * Implements hook_field_widget_form().
 */
function babble_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $element['access'] = array(
    '#type' => 'select',
    '#title' => t('Comment access'),
    '#options' => array(
      BABBLE_COMMENTS_OPEN => t('Open'),
      BABBLE_COMMENTS_CLOSED => t('Closed'),
      BABBLE_COMMENTS_HIDDEN => t('Hidden'),
    ),
    '#default_value' => isset($items[$delta]['access']) ? $items[$delta]['access'] : (isset($instance['settings']['access']) ? $instance['settings']['access'] : BABBLE_COMMENTS_CLOSED),
  );

  return $element;
}

/**
 * Implements hook_field_settings_form().
 */
function babble_field_settings_form($field, $instance, $has_data) {
  $form = array();

  if (user_access('administer babble')) {
    $bundles = field_info_bundles();

    $shared = array();
    foreach ($field['bundles'] as $entity_type => $type_bundles) {
      foreach ($type_bundles as $bundle) {
        $shared[] = module_exists('field_ui') ? l($bundles[$entity_type][$bundle]['label'], _field_ui_bundle_admin_path($entity_type, $bundle) . '/fields') : $bundles[$entity_type][$bundle]['label'];
      }
    }

    $form['link'] = array(
      '#type' => 'item',
      '#title' => t('Bundle settings'),
      '#markup' => t('You can change the behavior of this field on the <a href="!link">bundle settings page</a>.', array('!link' => url('admin/structure/babble/' . strtr($field['field_name'], array('_' => '-'))))),
    );

    if (!empty($shared)) {
      $form['link']['#description'] = t('This field is currently used by !bundles.', array('!bundles' => implode(', ', $shared)));
    }
  }

  return $form;
}

/**
 * Implements hook_field_instance_settings_form().
 */
function babble_field_instance_settings_form($field, $instance) {
  $form['access'] = array(
    '#type' => 'select',
    '#title' => t('Comment access'),
    '#description' => t('Default comment settings for new entities.'),
    '#options' => array(
      BABBLE_COMMENTS_OPEN => t('Open'),
      BABBLE_COMMENTS_CLOSED => t('Closed'),
      BABBLE_COMMENTS_HIDDEN => t('Hidden'),
    ),
    '#default_value' => isset($instance['settings']['access']) ? $instance['settings']['access'] : BABBLE_COMMENTS_CLOSED,
  );

  return $form;
}

/**
 * Implements hook_field_widget_error().
 */
function babble_field_widget_error($element, $error, $form, &$form_state) {
  form_error($element, $error['message']);
}

/**
 * Implements hook_field_delete().
 */
function babble_field_delete($entity_type, $entity, $field, $instance, $langcode, &$items) {
  if ($field['type'] == 'babble') {
    list($entity_id) = entity_extract_ids($entity_type, $entity);

    $query = new EntityFieldQuery();
    $query = $query->entityCondition('entity_type', 'babble')
      ->propertyCondition('entity_type', $entity_type)
      ->propertyCondition('entity_id', $entity_id)
      ->propertyCondition('field_name', $field['field_name']);

    $result = $query->execute();
    if (!empty($result['babble'])) {
      entity_delete_multiple('babble', array_keys($result['babble']));
    }
  }
}

/**
 * Implements hook_field_delete_instance().
 */
function babble_field_delete_instance($instance) {
  $field = field_info_field($instance['field_name']);

  if ($field['type'] == 'babble') {
    $query = new EntityFieldQuery();
    $query = $query->entityCondition('entity_type', 'babble')
      ->propertyCondition('entity_type', $instance['entity_type'])
      ->propertyCondition('field_name', $instance['field_name']);

    $result = $query->execute();
    if (!empty($result['babble'])) {
      entity_delete_multiple('babble', array_keys($result['babble']));
    }
  }
}

/**
 * Implements hook_field_delete_field().
 */
function babble_field_delete_field($field) {
  if ($field['type'] == 'babble') {
    // Rebuild the menu tree.
    variable_set('menu_rebuild_needed', TRUE);
  }
}

/**
 * Implements hook_field_create_instance().
 */
function babble_field_create_instance($instance) {
  $field = field_info_field($instance['field_name']);

  if ($field['type'] == 'babble') {
    // Per default we add a 'body' field to every new bundle.
    if (!field_info_instance('babble', 'babble_body', $instance['field_name'])) {
      if (!field_info_field('babble_body')) {
        $body_field = array(
          'field_name' => 'babble_body',
          'type' => 'text_long',
          'entity_types' => array('babble'),
          'module' => 'text',
          'settings' => array(),
          'cardinality' => 1,
        );

        field_create_field($body_field);
      }

      $body_instance = array(
        'field_name' => 'babble_body',
        'entity_type' => 'babble',
        'bundle' => $instance['field_name'],
        'label' => t('Comment'),
        'settings' => array(
          'text_processing' => 1,
        ),
        'required' => TRUE,
        'display' => array(
          'default' => array(
            'label' => 'hidden',
            'type' => 'text_default',
            'weight' => 1,
            'settings' => array(),
            'module' => 'text',
          ),
        ),
        'widget' => array(
          'type' => 'text_textarea',
          'settings' => array(
            'rows' => 5,
          ),
          'weight' => 1,
          'module' => 'text',
        ),
        'description' => '',
      );

      field_create_instance($body_instance);
    }

    // Per default we add a 'subject' field to every new bundle.
    if (!field_info_instance('babble', 'babble_subject', $instance['field_name'])) {
      if (!field_info_field('babble_subject')) {
        $subject_field = array(
          'field_name' => 'babble_subject',
          'type' => 'text',
          'entity_types' => array('babble'),
          'module' => 'text',
          'settings' => array(),
          'cardinality' => 1,
        );

        field_create_field($subject_field);
      }

      $subject_instance = array(
        'field_name' => 'babble_subject',
        'entity_type' => 'babble',
        'bundle' => $instance['field_name'],
        'label' => t('Subject'),
        'settings' => array(),
        'required' => TRUE,
        'display' => array(
          'default' => array(
            'label' => 'hidden',
            'type' => 'hidden',
            'weight' => 0,
            'settings' => array(),
            'module' => 'text',
          ),
        ),
        'widget' => array(
          'type' => 'text_textfield',
          'settings' => array(),
          'weight' => 0,
          'module' => 'text',
        ),
        'description' => '',
      );

      field_create_instance($subject_instance);
    }

    // Configure the 'subject' field as the entity title field.
    $field['settings']['title_field'] = 'babble_subject';
    field_update_field($field);

    // Rebuild the menu tree.
    variable_set('menu_rebuild_needed', TRUE);
  }
}