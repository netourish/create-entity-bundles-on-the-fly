<?php
/**
 * Travel controller class.
 */
class TravelController extends EntityAPIController {
  /**
   * Build a structured array representing the entity's content.
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {

    $wrapper = entity_metadata_wrapper('travel', $entity);

    $content['transport'] = array(
      '#theme' => 'field',
      '#weight' => 0,
      '#title' => t('Transport'),
      '#access' => TRUE,
      '#label_display' => 'above',
      '#view_mode' => 'full',
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_fake_transport',
      '#field_type' => 'text',
      '#entity_type' => 'travel',
      '#bundle' => $entity->type,  //For bundle here type has to be used.
      '#items' => array(array('value' => $wrapper->transport->value())),
      '#formatter' => 'text_default',
      0 => array('#markup' => check_plain($wrapper->transport->value())),
    );

    // Make "title" and "description" properties themed like default fields.
    $content['source_from'] = array(
      '#theme' => 'field',
      '#weight' => 0,
      '#title' => t('Source From'),
      '#access' => TRUE,
      '#label_display' => 'above',
      '#view_mode' => 'full',
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_fake_source_from',
      '#field_type' => 'text',
      '#entity_type' => 'travel',
      '#bundle' => $entity->type,
      '#items' => array(array('value' => $wrapper->source_from->value())),
      '#formatter' => 'text_default',
      0 => array('#markup' => check_plain($wrapper->source_from->value())),
    );

    $content['destination_to'] = array(
      '#theme' => 'field',
      '#weight' => 0,
      '#title' => t('Source From'),
      '#access' => TRUE,
      '#label_display' => 'above',
      '#view_mode' => 'full',
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_fake_source_from',
      '#field_type' => 'text',
      '#entity_type' => 'travel',
      '#bundle' => $entity->type,
      '#items' => array(array('value' => $wrapper->destination_to->value())),
      '#formatter' => 'text_default',
      0 => array('#markup' => check_plain($wrapper->destination_to->value())),
    );

    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }
}

/**
 * Travel Type controller class.
 */
class TravelTypeController extends EntityAPIControllerExportable {
//class TravelTypeController extends EntityAPIController {
  /**
   * Rebuild menu after editing entity bundles.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    parent::save($entity, $transaction);
    // Rebuild menu registry.
    variable_set('menu_rebuild_needed', TRUE);
  }
}

/**
 * Travel Type UI controller class.
 */
class TravelTypeUiController extends EntityDefaultUIController {
  /**
   * Override hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage Travel Types.';
    return $items;
  }
}
