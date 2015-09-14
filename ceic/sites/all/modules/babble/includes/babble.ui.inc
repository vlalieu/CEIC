<?php

/**
 * @file
 * Provides the UI controller and the entity form for the babble entity.
 */

/**
 * Button submit function: handle the 'Delete' button on the comment form.
 */
function babble_form_delete_submit($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }

  $babble = $form_state['babble'];
  $form_state['redirect'] = array('babble/' . $babble->bid . '/delete', array('query' => $destination));
}

/**
 * Form constructor for the comment delete confirmation form.
 */
function babble_delete_form($form, &$form_state, Babble $babble) {
  $form_state['babble'] = $babble;

  $label = entity_label('babble', $babble);
  $redirect = entity_uri($babble->entity_type, entity_load_single($babble->entity_type, $babble->entity_id));

  return confirm_form($form, t('Are you sure you want to delete %label?', array('%label' => $label)), $redirect);
}

/**
 * Form submit handler for the delete confirmation form.
 */
function babble_delete_form_submit($form, &$form_state) {
  $babble = $form_state['babble'];
  $label = entity_label('babble', $babble);

  $uri = entity_uri($babble->hostEntityType(), $babble->hostEntity());
  $form_state['redirect'] = array($uri['path'], $uri['options']);

  entity_delete('babble', $babble->bid);
  drupal_set_message(t('Deleted %label.', array('%label' => $label)));
}

/**
 * Menu callback for publishing a specified comment.
 *
 * @param $babble
 *   A comment entity.
 */
function babble_approve(Babble $babble) {
  $babble->status = BABBLE_PUBLISHED;
  $babble->save();

  drupal_set_message(t('Comment approved.'));

  $uri = entity_uri('babble', $babble);
  drupal_goto($uri['path'], $uri['options']);
}

/**
 * Menu system callback to display a single comment on a page.
 */
function babble_permalink(Babble $entity) {
  if ($host = $entity->hostEntity()) {
    $uri = entity_uri($entity->hostEntityType(), $host);

    // Set the active menu item ourselves so that the final page callback
    // behaves as it would when visiting the page directly.
    menu_set_active_item($uri['path']);

    // Return the entity view, this will show the correct comment in context.
    return menu_execute_active_handler(NULL, FALSE);
  }

  drupal_not_found();
}

/**
 * Page callback for displaying a 'reply' page.
 */
function babble_reply_page(Babble $babble) {
  $output['parent'] = babble_view(array($babble), 'reply', NULL, TRUE);

  $reply = entity_create('babble', array(
    'entity_type' => $babble->entity_type,
    'entity_id' => $babble->entity_id,
    'field_name' => $babble->field_name,
  ));

  $output['reply'] = drupal_get_form('babble_form', $reply);

  drupal_set_title(t('Reply to @title', array('@title' => entity_label('babble', $babble))), PASS_THROUGH);

  return $output;
}