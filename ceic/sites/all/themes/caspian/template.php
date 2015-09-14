<?php

/**
 * @file
 * This file is empty by default because the base theme chain (Alpha & Omega) provides
 * all the basic functionality. However, in case you wish to customize the output that Drupal
 * generates through Alpha & Omega this file is a good place to do so.
 * 
 * Alpha comes with a neat solution for keeping this file as clean as possible while the code
 * for your subtheme grows. Please read the README.txt in the /preprocess and /process subfolders
 * for more information on this topic.
 */
 
 
 
/**
 * Make the main menu inline
 */
function caspian_menu_tree__main_menu($variables) {
  return '<ul class="links inline clearfix">' . $variables['tree'] . '</ul>';
}



/**
 * Allow html.tpl.php pages by path
 */
function caspian_preprocess_html (&$vars) {
	if (module_exists('path')) {
    $alias = drupal_get_path_alias(str_replace('/edit','',$_GET['q']));
    if ($alias != $_GET['q']) {
      $template_filename = 'html';
      foreach (explode('/', $alias) as $path_part) {
        $template_filename = $template_filename . '__' . $path_part;
        $vars['theme_hook_suggestions'][] = $template_filename;
      }
    }
  }
}

/**
 * Search box tweak
 */
function caspian_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'search_block_form') {


    // Alternative (HTML5) placeholder attribute instead of using the javascript
    $form['search_block_form']['#attributes']['placeholder'] = t('Search...');
  }
} 