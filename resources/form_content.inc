<?php

/**
 * Gets access rules.
 *
 * @param $op
 *  $op - Permission requested
 * @return Boolean
 *  Boolean TRUE if the current user has the requested permission
 */
function _form_content_resource_access($op = 'view', $args = array()) {
  switch ($op) {
    case 'view':
      return user_access('administer users');
      break;
  }
}


/**
 * Gets all resource definitions.
 *
 * @return array
 *  An array containing all resources.
 */
function form_content_resources() {
  $resource = array(
    'retrieve' => array(
      'callback' => '_form_content_resources_retrieve',
        'args' => array(
          array(
            'name' => 'type',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'string',
            'description' => 'Provide name of content type to get form structure',
          ),
        ),
        'access callback' => '_form_content_resource_access',
        'access arguments' => array('view'),
        'access arguments append' => TRUE,
    ),
  );
  return $resource;
}


/**
 * Returns the form structure. 
 *
 * @param $type
 *   $type - content type name
 * @return
 *   form structure
 */
function _form_content_resources_retrieve($type) 
{ 
 return getNodeField($type);
  module_load_include('inc', 'node', 'node.pages');
  global $user;

  $types = node_type_get_types();
  $type = isset($type) ? str_replace('-', '_', $type) : NULL;

  // If a node type has been specified, validate its existence.
  if (isset($types[$type]) && node_access('create', $type)) {
    // Initialize settings:
    $node = new stdClass();
    $node->uid = $user->uid;
    $node->name = $user->name;
    $node->type = $type;
    $node->language = 'und';

    $form = drupal_get_form($type.'_node_form', $node);
    if($form) {
      $field_list = array();
      $field_list['title'] = $form['title'];
      foreach($form as $key => $val) { 
         if($key == 'title' || $key == 'body' || strpos($key, 'field') > -1) { 
           if($val['und'][0]['value']) { 
             $field_list[$key] = $val['und']['0']['value'];
           }
           else if($val['und']) {
             $field_list[$key] = $val['und'];
           }
         }
       }
      if($field_list) {
        foreach($field_list as $k1 => $v1) {
           $field_list['form_fields'][] = $k1;
        }
      } 

      return $field_list; 
    } 
  }

  return FALSE;
}

function getNodeField($type)
{
  $query = new EntityFieldQuery();
$query->entityCondition('entity_type', 'node')
  ->entityCondition('bundle', $type);
//  ->propertyCondition('status', 1);
//  ->fieldCondition('field_story_image', 'fid', 'NULL', '!=');
$result = $query->execute();
 
if (isset($result['node'])) {
    $page = $result['node'];
 
    // At first we need to get field's id. If you already know field id, you can ommit this step
    // Get all fields attached to a given node type
    $fields = field_info_instances('node', $type);
    print_r($fields); exit;
    $field_id = $fields['field_student_name']['field_id'];

    return $fields;
    $news_items_nids = array_keys($result['node']);
    $news_items = entity_load('node', $news_items_nids);

    return $news_items;
   }
}