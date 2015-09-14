<?php

/**
 * Entity class for the babble entity.
 */
class Babble extends Entity {

  /**
   * Overrides Entity::__construct().
   */
  public function __construct($values = array()) {
    parent::__construct($values, 'babble');
  }

  /**
   * Overrides Entity::defaultLabel().
   */
  public function defaultLabel() {
    $field = field_info_field($this->field_name);

    if ($field && isset($field['settings']['title_field']) && $field['settings']['title_field'] != BABBLE_TITLE_FIELD_UNDEFINED) {
      if (field_info_instance('babble', $field['settings']['title_field'], $this->field_name)) {
        if ($items = field_get_items('babble', $this, $field['settings']['title_field'])) {
          $title = field_view_value('babble', $this, $field['settings']['title_field'], $items[0]);

          return filter_xss(drupal_render($title));
        }
      }
    }
  }

  /**
   * Overrides Entity::defaultUri().
   */
  public function defaultUri() {
    return array(
      'path' => 'babble/' . $this->bid,
      'options' => array(
        'fragment' => 'babble-' . $this->bid,
      ),
    );
  }

  /**
   * Sets the host. Only possible during the creation of a comment.
   */
  public function setHost($entity_type, $entity, $field_name) {
    if (!empty($this->is_new)) {
      list($this->entity_id) = entity_extract_ids($entity_type, $entity);
      $this->entity_type = $entity_type;
      $this->field_name = $field_name;
      $this->hostEntity = $entity;
    }
    else {
      throw new Exception('The host entity may be set only during creation of a comment.');
    }
  }

  /**
   * Returns the host entity of a comment.
   */
  public function hostEntity() {
    if (isset($this->entity_id) && isset($this->entity_type)) {
      if (!isset($this->hostEntity)) {
        $this->hostEntity = entity_load_single($this->entity_type, $this->entity_id);
      }

      return $this->hostEntity;
    }
  }

  /**
   * Returns the entity type of the host entity of a comment.
   */
  public function hostEntityType() {
    if (isset($this->entity_type)) {
      return $this->entity_type;
    }
  }

  /**
   * Returns the id of the host entity of a comment.
   */
  public function hostEntityId() {
    if ($this->entity_id) {
      return $this->entity_id;
    }
  }

}