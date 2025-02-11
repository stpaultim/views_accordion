<?php
/**
 * @file
 * Provide an accordion style plugin for Views. This file is autoloaded by views.
 */

/**
 * Implements views_plugin_style().
 */
class views_accordion_style_plugin extends views_plugin_style {

  /**
   * Set default options.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['use-grouping-header'] = array('default' => 0);
    $options['collapsible'] = array('default' => 0);
    $options['row-start-open'] = array('default' => 0);
    $options['animated'] = array('default' => 'swing');
    $options['animation_time'] = array('default' => 300);
    $options['autoheight'] = array('default' => 1);
    $options['event'] = array('default' => 'click');
    $options['fillspace'] = array('default' => 0);
    $options['navigation'] = array('default' => 0);
    $options['clearstyle'] = array('default' => 0);
    $options['disableifone'] = array('default' => 0);

    /*
     * @TODO:
     * 1. Figure out if/how to provide support for jQuery UI themes
     *    See http://drupal.org/node/388384 perhaps
     * 2. Provide support for missing accordion options:
     *    disabled
     *    icons
     *    navigationFilter
     * See http://jqueryui.com/demos/accordion/
     */
    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['grouping']['#prefix'] = '<div class="form-item">' . t('<strong>IMPORTANT:</strong> The <em>first field</em> in order of appearance <em>will</em> be the one used as the "header" or "trigger" of the accordion action.') . '</div>';
    // Available valid options for grouping (used for use-grouping-header
    // #dependency).
    $options = array();
    foreach ($this->display->handler->get_handlers('field') as $field => $handler) {
      $options[] = $field;
    }
    // Find out how many items the display is currently configured to show
    // (row-start-open).
    $maxitems = $this->display->handler->get_option('items_per_page');
    // If items_per_page is set to unlimitted (0), 10 rows will be what the user
    // gets to choose from.
    $maxitems = ($maxitems == 0) ? 10 : $maxitems;

    // Setup our array of options for choosing which row should start opened
    // (row-start-open).
    $rsopen_options = array();
    for ($i = 1; $i <= $maxitems; $i++) {
      $rsopen_options[] = t('Row') . $i;
    }
    $rsopen_options['none'] = t('None');
    $rsopen_options['random'] = t('Random');

    // The easing effects from jQuery UI.
    $animated_options = array(
      'none' => t('None'),
      'swing' => t('Swing'),
      'linear' => t('Linear'),
      'easeInQuart' => t('easeInQuart'),
      'easeOutQuart' => t('easeOutQuart'),
      'easeInOutQuart' => t('easeInOutQuart'),
      'easeInExpo' => t('easeInExpo'),
      'easeOutExpo' => t('easeOutExpo'),
      'easeInOutExpo' => t('easeInOutExpo'),
      'easeInBack' => t('easeInBack'),
      'easeOutBack' => t('easeOutBack'),
      'easeInOutBack' => t('easeInOutBack'),
      'easeInQuad' => t('easeInQuad'),
      'easeOutQuad' => t('easeOutQuad'),
      'easeInOutQuad' => t('easeInOutQuad'),
      'easeInQuint' => t('easeInQuint'),
      'easeOutQuint' => t('easeOutQuint'),
      'easeInOutQuint' => t('easeInOutQuint'),
      'easeInCirc' => t('easeInCirc'),
      'easeOutCirc' => t('easeOutCirc'),
      'easeInOutCirc' => t('easeInOutCirc'),
      'easeInBounce' => t('easeInBounce'),
      'easeOutBounce' => t('easeOutBounce'),
      'easeInOutBounce' => t('easeInOutBounce'),
      'easeInCubic' => t('easeInCubic'),
      'easeOutCubic' => t('easeOutCubic'),
      'easeInOutCubic' => t('easeInOutCubic'),
      'easeInSine' => t('easeInSine'),
      'easeOutSine' => t('easeOutSine'),
      'easeInOutSine' => t('easeInOutSine'),
      'easeInElastic' => t('easeInElastic'),
      'easeOutElastic' => t('easeOutElastic'),
      'easeInOutElastic' => t('easeInOutElastic'),
    );

    $form['use-grouping-header'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use the group header as the Accordion header'),
      '#default_value' => $this->options['use-grouping-header'],
      '#description' => t("If checked, the Group's header will be used to open/close the accordion."),
      '#dependency' => array('edit-style-options-grouping' => $options),
    );
    $form['row-start-open'] = array(
      '#type' => 'select',
      '#title' => t('Row to display opened on start'),
      '#default_value' => $this->options['row-start-open'],
      '#description' => t('Choose which row should start opened when the accordion first loads. If you want all to start closed, choose "None", and make sure to have "Allow for all rows to be closed" on below.'),
      '#options' => $rsopen_options,
    );
    $form['collapsible'] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow for all rows to be closed'),
      '#default_value' => $this->options['collapsible'],
      '#description' => t('If you check this on, when the user clicks on an opened item, it will close.'),
    );
    $form['animated'] = array(
      '#type' => 'select',
      '#title' => t('Animation effect'),
      '#default_value' => $this->options['animated'],
      '#description' => t('Choose what animation effect you would like to see, or "None" to disable it.'),
      '#options' => $animated_options,
    );
    $form['animation_time'] = array(
      '#type' => 'number',
      '#title' => t('Animation time'),
      '#default_value' => $this->options['animation_time'],
      '#min' => 0,
      '#description' => t('The animation duration in milliseconds'),
    );
    $form['autoheight'] = array(
      '#type' => 'checkbox',
      '#title' => t('Autoheight'),
      '#default_value' => $this->options['autoheight'],
      '#description' => t('If set, the highest content part is used as height reference for all other parts. Provides more consistent animations.'),
    );
    $form['event'] = array(
      '#type' => 'select',
      '#title' => t('Event'),
      '#default_value' => $this->options['event'],
      '#description' => t('The event on which to trigger the accordion.'),
      '#options' => array(
        'click' => t('Click'),
        'mouseover' => t('Mouseover'),
      ),
    );
    $form['fillspace'] = array(
      '#type' => 'checkbox',
      '#title' => t('Fillspace'),
      '#default_value' => $this->options['fillspace'],
      '#description' => t('If set, the accordion completely fills the height of the parent element. Overrides autoheight.'),
    );
    $form['navigation'] = array(
      '#type' => 'checkbox',
      '#title' => t('Navigation'),
      '#default_value' => $this->options['navigation'],
      '#description' => t('If set, looks for the anchor that matches location.href and activates it. Great for href-based state-saving. Use navigationFilter to implement your own matcher.'),
    );
    $form['clearstyle'] = array(
      '#type' => 'checkbox',
      '#title' => t('Clearstyle'),
      '#default_value' => $this->options['clearstyle'],
      '#description' => t("If set, clears height and overflow styles after finishing animations. This enables accordions to work with dynamic content. Won't work together with autoHeight."),
    );
    $form['disableifone'] = array(
      '#type' => 'checkbox',
      '#title' => t('Disable if only one result'),
      '#default_value' => $this->options['disableifone'],
      '#description' => t("If set, the accordion will not be shown when there are less than 2 results."),
    );
  }

  /**
   * Necessary to prevent markup mayhem.
   */
  public function pre_render($result) {
    backdrop_add_library('system', 'ui.accordion');
    backdrop_add_js(backdrop_get_path('module', 'views_accordion') . '/js/views-accordion.js');
    // No need do anything if we are using the grouped field as the header.
    if ($this->options['use-grouping-header']) {
      return;
    }
    // Find out about the header field options.
    $fields = array_values($this->display->handler->get_option('fields'));
    $header_class = 'views-accordion-header';
    // Add header class to first not-excluded field.
    foreach ($fields as $field) {
      if (!isset($field['exclude']) || ($field['exclude'] == 0)) {
        // Setup our wrapper class.
        // If the user configured its own class, set that up with our own class.
        if (!empty($field['element_wrapper_class'])) {
          $header_wrapper_class = $field['element_wrapper_class'] . ' ' . $header_class;
        }
        // Otherwise use our own class only.
        else {
          $header_wrapper_class = $header_class;
        }
        // Setup the view to use our processed wrapper class.

        $this->view->field[$field['id']]->options['element_wrapper_class'] = $header_wrapper_class;

        // Make sure we are using a div for markup at least.
        if (empty($field['element_wrapper_type'])) {
          $this->view->field[$field['id']]->options['element_wrapper_type'] = 'div';
        }
        break;
      }
    }
  }

  /**
   * Render the display in this style.
   */
  public function render() {
    $output = parent::render();

    if ($this->options['disableifone'] == '1') {
      if (count($this->rendered_fields) < 2) {
        return $output;
      }
    }

    // Add the appropriate effect library if necessary.
    $effect = $this->options['animated'];
    if (($effect !== 'none')) {
      // For now we only use ui core effects library, which provides the easing
      // effects.This switch is left here in case we actualy need to load any
      // other libraries.
      switch ($effect) {
        default:
          $library = 'effects';
          break;
      }
      if (isset($library)) {
        backdrop_add_library('system', 'effects');
      }
    }

    // Preparing the js variables and adding the js to our display.
    // We do it here so we dont have it run once every group.
    $view_settings['collapsible'] = $this->options['collapsible'];

    if ($this->options['row-start-open'] == 'random') {
      $view_settings['rowstartopen'] = 'random';
    }
    else {
      $view_settings['rowstartopen'] = ($this->options['row-start-open'] == 'none') ? FALSE : (int) $this->options['row-start-open'];
    }
    $view_settings['animated'] = ($this->options['animated'] == 'none') ? FALSE : $this->options['animated'];
    $view_settings['duration'] = (int) $this->options['animation_time'];
    $view_settings['autoheight'] = $this->options['autoheight'];
    $view_settings['event'] = $this->options['event'];
    $view_settings['fillspace'] = $this->options['fillspace'];
    $view_settings['navigation'] = $this->options['navigation'];
    $view_settings['clearstyle'] = $this->options['clearstyle'];

    $view_settings['grouping'] = $this->options['grouping'] ? 1 : 0;

    $view_settings['display'] = $this->view->current_display;
    $view_settings['viewname'] = $this->view->name;
    $view_settings['usegroupheader'] = $view_settings['grouping'] ? $this->options['use-grouping-header'] : 0;

    $accordion_id = 'views-accordion-' . $this->view->name . '-' . $this->view->current_display;

    if ($view_settings['usegroupheader'] == 1) {
      $view_settings['header'] = 'h3.' . $accordion_id . '-header';
    }

    // Used to get the first field to be used as the accordion header.
    if ($view_settings['usegroupheader'] == 0) {
      $view_settings['header'] = '.views-accordion-header';
    }

    // Generate unique row IDs.
    list($entity_type, $entities) = $this->view->query->get_result_entities($this->view->result);
    if (count($entities)) {
      // Generate a rowid for each result using [entity_type]-[entity_id].
      foreach ($entities as $entity) {
        $view_settings['rowids'][] = $entity_type . '-' . $entity->{$this->view->base_field};
      }
    }
    // The view does not contain entities related to the base table/field.
    else {
      if ($view_settings['navigation']) {
        // Display an error if deep links are enabled but cannot be generated.
        backdrop_set_message('For views accordion to generate deep links unique to each view result, at least one field related to the base table must be included in the query. Base table fields can be hidden if it is desired that they be excluded in the output.', 'error');
      }
      for ($i = 0; $i < count($this->view->result); ++$i) {
        $view_settings['rowids'][] = $i;
      }
    }

    backdrop_add_js(array('views_accordion' => array($accordion_id => $view_settings)), 'setting');

    return $output;
  }

  /**
   * {@inheritdoc}
   */
  public function validate() {
    $errors = parent::validate();
    if ($this->display->handler->get_option('row_plugin') !== 'fields') {
      $row_plugin = $this->display->handler->get_plugin('row');
      $errors[] = t('Views accordion requires Fields as row style, but the view <em>@view_name</em> is configured with <em>@row_plugin_title</em> as a row style.', array(
        '@view_name' => $this->view->name,
        '@row_plugin_title' => $row_plugin->definition['title'],
      ));
    }
    return $errors;
  }

}
