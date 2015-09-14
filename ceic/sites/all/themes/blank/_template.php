<?php

function hook_openlayers_quick_query_info($view) {
  if ($view->name == 'openlayers_taxonomy') {

    // Add projection and wkt information
    $info['projection'] = '4326';
    $info['wkt_field'] = 'field_geolocation';

    // Add needed fields
    $info['fields'][] = array(
      'field_name' => 'field_geolocation'
    );

     $info['attributes'] = array(
      'nid' => 'nid',
      'name' => 'node_link_fast', // Special very fast node_link linking the title using just node/<nid> with no aliases
    );

    return $info;
  }
}