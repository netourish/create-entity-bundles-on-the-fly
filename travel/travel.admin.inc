<?php

/**
 * @file
 * Callbacks for administrating entities.
 */

/**
 * Choose bundle of entity to add.
 * 
 * @return array
 *   Array describing a list of bundles to render.
 */
function travel_choose_bundle() {
  // Show list of all existing entity bundles.
  $items = array();
  foreach (travel_type_load_multiple() as $entity_type_key => $entity_type) {
    $items[] = l(entity_label('travel_type', $entity_type), 'travel/add/' . $entity_type_key);
  }
  return array(
    'list' => array(
      '#theme' => 'item_list',
      '#items' => $items,
      '#title' => t('Choose type of entity to add.'),
    ),
  );
}

/**
 * Form constructor for the entity add form.
 * 
 * @param string $type
 *   Entity type (bundle) to add.
 * 
 * @return array
 *   Entity edit form.
 */
function travel_add($type) {
  $entity_type = travel_type_load_multiple($type);
  drupal_set_title(t('Create @name', array('@name' => entity_label('entity_type', $entity_type))));

  // Return form for the given entity bundle.
  $entity = entity_create('travel', array('type' => $type));
  $output = drupal_get_form('travel_form', $entity);
  return $output;
}

/**
 * Form constructor for the entity edit form.
 * 
 * In this form we shall manually define all form elements related to editable
 * properties of the entity.
 * 
 * @param object $entity
 *   Entity to edit.
 * 
 * @return array
 *   Entity edit form.
 */
function travel_form($form, &$form_state, $entity) {
  // Store the entity in the form.
  $form_state['entity'] = $entity;

  // Describe all properties of the entity which shall be shown on the form.
  $wrapper = entity_metadata_wrapper('travel', $entity);

  $form['transport'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Transport:'),
    '#default_value' => $wrapper->transport->value(),
    '#weight' => -20,
  );

  $form['source_from'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Source From:'),
    '#default_value' => $wrapper->source_from->value(),
    '#weight' => -16,
  );
  $form['destination_to'] = array(
    '#type' => 'textfield',
    '#title' => t('Destination To:'),
    '#default_value' => $wrapper->destination_to->value(),
    '#weight' => -10,
  );

  // Add fields of the entity to the form.
  field_attach_form('travel', $entity, $form, $form_state);

  // Add some buttons.
  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }
  $form['actions'] = array(
    '#weight' => 100,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save entity'),
    '#submit' => $submit + array('travel_form_submit'),
  );
  $entity_id = entity_id('travel', $entity);
  if (!empty($entity_id)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('travel_form_submit_delete'),
    );
  }

  return $form;
}

/**
 * Entity edit form "Save" submit handler.
 */
function travel_form_submit($form, &$form_state) {
  // Automatically get edited entity from the form and save it.
  // @see entity_form_submit_build_entity()
  $entity = $form_state['entity'];
  entity_form_submit_build_entity('travel', $entity, $form, $form_state);
  entity_save('travel', $entity);

  // Redirect user to edited entity page.
  $entity_uri = entity_uri('travel', $entity);
  $form_state['redirect'] = $entity_uri['path'];
  drupal_set_message(t('Entity %title saved.', array('%title' => entity_label('travel', $entity))));
}

/**
 * Entity edit form "Delete" submit handler.
 */
function travel_form_submit_delete($form, &$form_state) {
  // Redirect user to "Delete" URI for this entity.
  $entity = $form_state['entity'];
  $entity_uri = entity_uri('travel', $entity);
  $form_state['redirect'] = $entity_uri['path'] . '/delete';
}

/**
 * Form constructor for the entity delete confirmation form.
 * 
 * @param object $entity
 *   Entity to delete.
 * 
 * @return array
 *   Confirmation form.
 */
function travel_delete_form($form, &$form_state, $entity) {
  // Store the entity in the form.
  $form_state['entity'] = $entity;

  // Show confirm dialog.
  $entity_uri = entity_uri('travel', $entity);
  $message = t('Are you sure you want to delete entity %title?', array('%title' => entity_label('travel', $entity)));
  return confirm_form(
    $form,
    $message,
    $entity_uri['path'],
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Entity "Confirm delete" form submit handler.
 */
function travel_delete_form_submit($form, &$form_state) {
  // Delete the entity.
  $entity = $form_state['entity'];
  entity_delete('travel', entity_id('travel', $entity));

  // Redirect user.
  drupal_set_message(t('Entity %title deleted.', array('%title' => entity_label('travel', $entity))));
  $form_state['redirect'] = '<front>';
}

/**
 * Form constructor for the entity type edit form.
 * 
 * In this form we shall manually define all form elements related to editable
 * properties of the entity.
 * 
 * @param object $entity_type
 *   Enity type.
 * @param string $op
 *   Operation to perform on entity type.
 * 
 * @return array
 *   Entity type edit form.
 */
function travel_type_form($form, &$form_state, $entity_type, $op = 'edit') {
  // Handle the case when cloning is performed.
  if ($op == 'clone') {
    $entity_type->label .= ' (cloned)';
    $entity_type->type = '';
  }

  // Describe all properties of the entity which shall be shown on the form.
  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $entity_type->label,
    '#description' => t('The human-readable name of this entity type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($entity_type->type) ? $entity_type->type : '',
    '#maxlength' => 32,
    //'#disabled' => $entity_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'travel_type_load_multiple',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this entity type. It must only contain lowercase letters, numbers, and underscores.'),
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($entity_type->description) ? $entity_type->description : '',
    '#description' => t('Description about the entity type.'),
  );

  // Add some buttons.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save entity type'),
    '#weight' => 40,
  );
  //if (!$entity_type->isLocked() && $op != 'add' && $op != 'clone') {
  $entity_id = entity_id('travel', $entity);
  if (!empty($entity_id)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete entity type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('travel_type_form_submit_delete'),
    );
  }

  return $form;
}

/**
 * Entity type edit form "Save" submit handler.
 */
function travel_type_form_submit(&$form, &$form_state) {
  // Automatically get edited entity type from the form and save it.
  // @see entity_ui_form_submit_build_entity()
  $entity_type = entity_ui_form_submit_build_entity($form, $form_state);
  entity_save('travel_type', $entity_type);

  // Redirect user.
  $form_state['redirect'] = 'admin/structure/travel';
}

/**
 * Entity edit form "Delete" submit handler.
 */
function travel_type_form_submit_delete(&$form, &$form_state) {
  // Redirect user to "Delete" URI for this entity type.
  $form_state['redirect'] = 'admin/structure/travel/' . $form_state['travel_type']->type . '/delete';
}

/**
 * Form constructor for the entity type delete confirmation form.
 * 
 * @param object $entity_type
 *   Entity type to delete.
 * 
 * @return array
 *   Confirmation form.
 */
function travel_type_form_delete_confirm($form, &$form_state, $entity_type) {
  // Store the entity in the form.
  $form_state['entity_type'] = $entity_type;

  // Show confirm dialog.
  $message = t('Are you sure you want to delete entity type %title?', array('%title' => entity_label('entity_type', $entity_type)));
  return confirm_form(
    $form,
    $message,
    'travel/' . entity_id('travel_type', $entity_type),
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Entity type "Confirm delete" form submit handler.
 */
function travel_type_form_delete_confirm_submit($form, &$form_state) {
  // Delete the entity type.
  $entity_type = $form_state['entity_type'];
  entity_delete('travel_type', entity_id('travel_type', $entity_type));

  // Redirect user.
  drupal_set_message(t('@type %title has been deleted.', array('@type' => $entity_type->type, '%title' => $entity_type->label)));
  $form_state['redirect'] = 'admin/structure/travel';
}
