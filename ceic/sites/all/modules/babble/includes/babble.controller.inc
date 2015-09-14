<?php

/**
 * Entity controller for the babble entity.
 */
class BabbleController extends EntityAPIController {

  /**
   * Overrides EntityAPIController::save().
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    $entity->changed = REQUEST_TIME;

    if (!empty($entity->is_new)) {
      $entity->created = REQUEST_TIME;

      if (!isset($entity->uid)) {
        $entity->uid = $GLOBALS['user']->uid;
      }

      if (!isset($entity->language)) {
        $entity->language = entity_language($entity->hostEntityType(), $entity->hostEntity());
      }

      $entity->hostname = ip_address();
    }

    parent::save($entity, $transaction);
  }

}