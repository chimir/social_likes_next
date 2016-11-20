<?php
/**
 * @file
 * Administration pages for the Social Likes module.
 */

/**
 * Returns settings form.
 */
function social_likes_admin_settings() {
  $form['settings'] = array(
    '#type' => 'vertical_tabs',
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'social_likes') . '/css/social-likes.admin.css'),
    ),
  );

  $form['settings']['appearance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Appearance'),
  );

  $form['settings']['appearance']['social_likes_skin'] = array(
    '#type' => 'radios',
    '#title' => t('Skin'),
    '#description' => t('Choosing skin.'),
    '#options' => array(
      'flat' => t('Flat'),
      'light' => t('Light'),
      'birman' => t('Birman'),
      'custom' => t('Custom'),
    ),
    '#default_value' => variable_get('social_likes_skin', 'flat'),
  );

  $form['settings']['appearance']['social_likes_custom'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom CSS'),
    '#description' => t('Path to the custom skin CSS file relative to Drupal root directory'),
    '#default_value' => variable_get('social_likes_custom', 'sites/all/modules/social_likes/css/social-likes_example.css'),
    '#states' => array(
      'visible' => array(
        ':input[name="social_likes_skin"]' => array(
          'value' => 'custom',
        ),
      ),
    ),
  );

  $form['settings']['appearance']['social_likes_look'] = array(
    '#type' => 'checkbox',
    '#title' => t('Vertical'),
    '#description' => t('All buttons in a column.'),
    '#default_value' => variable_get('social_likes_look', FALSE),
  );

  $form['settings']['appearance']['social_likes_labels'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remove labels'),
    '#description' => t('Remove the labels.'),
    '#default_value' => variable_get('social_likes_labels', FALSE),
  );

  $form['settings']['display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display output'),
  );

  $social_networks = social_likes_social_networks_list();
  $header = array('name' => t('Social networks'));
  $options = array();
  $weight = -10;
  $default_value = variable_get('social_likes_websites', drupal_map_assoc(array('facebook', 'twitter', 'vkontakte')));
  $form['settings']['display']['social_likes_websites'] = array();
  foreach (array_keys($default_value + $social_networks) as $website) {
    $options[$website] = array(
      'name' => $social_networks[$website],
      '#attributes' => array('class' => array('draggable')),
    );
    $form['settings']['display']['social_likes_websites'][$website . '_weight'] = array(
      '#tree' => FALSE,
      '#type' => 'weight',
      '#delta' => 10,
      '#default_value' => $weight++,
      '#attributes' => array('class' => array('social-likes-websites-weight')),
    );
  }

  $form['settings']['display']['social_likes_websites'] += array(
    '#type' => 'tableselect',
    '#title' => ('Social networks'),
    '#header' => $header,
    '#options' => $options,
    '#default_value' => $default_value,
    '#pre_render' => array('social_likes_websites_pre_render'),
  );

  $form['settings']['display']['social_likes_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
    '#description' => t('When buttons used on different page.') . ' ' .
      t('Install !link module to get list of all available tokens.',
        array('!link' => l(t('Token'), 'http://drupal.org/project/token', array('attributes' => array('target' => '_blank'))))),
    '#default_value' => variable_get('social_likes_url', '[node:url]'),
  );

  $form['settings']['display']['social_likes_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t("When differs from current page’s title.") . ' ' .
      t('Install !link module to get list of all available tokens.',
        array('!link' => l(t('Token'), 'http://drupal.org/project/token', array('attributes' => array('target' => '_blank'))))),
    '#default_value' => variable_get('social_likes_title', '[node:title]'),
  );

  $form['settings']['display']['social_likes_via'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Via'),
    '#description' => t('Username'),
    '#default_value' => variable_get('social_likes_via', ''),
    '#states' => array(
      'visible' => array(
        ':input[name="social_likes_websites[twitter]"]' => array(
          'checked' => TRUE,
        ),
      ),
    ),
  );

  $form['settings']['display']['social_likes_related'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Related'),
    '#description' => t('Username:Description'),
    '#default_value' => variable_get('social_likes_related', ''),
    '#states' => array(
      'visible' => array(
        ':input[name="social_likes_websites[twitter]"]' => array(
          'checked' => TRUE,
        ),
      ),
    ),
  );

  $form['settings']['display']['social_likes_image_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Image URL'),
    '#description' => t('Image URL for Pinterest (required)'),
    '#default_value' => variable_get('social_likes_image_url', ''),
    '#states' => array(
      'required' => array(
        ':input[name="social_likes_websites[pinterest]"]' => array(
          'checked' => TRUE,
        ),
      ),
      'visible' => array(
        ':input[name="social_likes_websites[pinterest]"]' => array(
          'checked' => TRUE,
        ),
      ),
    ),
  );

  if (module_exists('token')) {
    $form['settings']['display']['fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Tokens'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['settings']['display']['fieldset']['social_likes_token_tree'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('node'),
      '#global_types' => FALSE,
    );
  }

  $form['#validate'] = array('social_likes_admin_settings_validate');
  return system_settings_form($form);
}

/**
 * Pre-render callback for the social networks tableselect.
 */
function social_likes_websites_pre_render($element) {
  // Add weight column.
  $element['#header']['weight'] = t('Weight');
  foreach (array_keys($element['#options']) as $website) {
    $key = $website . '_weight';
    $element['#options'][$website]['weight'] = array(
      'data' => drupal_render($element[$key]),
    );
    unset($element[$key]);
  }

  // Assign id to the table.
  $table_id = 'social-likes-websites';
  $element['#attributes'] = array('id' => $table_id);
  drupal_add_tabledrag($table_id, 'order', 'sibling', 'social-likes-websites-weight');

  return $element;
}

function social_likes_admin_settings_validate($form, &$form_state) {
  $values = &$form_state['values'];

  $social_networks = social_likes_social_networks_list();
  // Remove weights so they are not saved as variables.
  foreach (array_keys($social_networks) as $website) {
    $websites[$website] = $values[$website . '_weight'];
    unset($values[$website . '_weight']);
  }
  asort($websites);

  $websites_values = $values['social_likes_websites'];
  unset($values['social_likes_websites']);

  foreach (array_keys($websites) as $website) {
    $values['social_likes_websites'][$website] = $websites_values[$website];
  }

  // Validating required invisible Image URL field.
  if ($image_uri_required = $values['social_likes_websites']['pinterest']) {
    if (empty($values['social_likes_image_url']) && $image_uri_required == 'pinterest') {
      form_set_error('social_likes_image_url', t('The field %field is required.', array('%field' => $form['settings']['display']['social_likes_image_url']['#title'])));
    }
  }
}
