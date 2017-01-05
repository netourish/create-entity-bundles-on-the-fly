<?php

/**
 * @file
 * Callbacks for viewing entities.
 */

/**
 * Entity view callback.
 * 
 * @param object $entity
 *   Entity to view.
 * 
 * @return array
 *   Renderable array.
 */
function travel_view($entity) {
  drupal_set_title(entity_label('travel', $entity));

  // Return automatically generated view page.
  return entity_view('travel', array(entity_id('travel', $entity) => $entity));
}
