<?php

/**
 * @file
 * Code to override field to add in auto option.
 */

class oa_core_oa_section_ref_views_handler_filter_in_operator extends views_handler_filter_in_operator {
  function init(&$view, &$options) {
    parent::init($view, $options);
    if (!empty($this->options['expose']['identifier']) && empty($this->options['expose']['oa_core_no_auto'])) {
      $id = $this->options['expose']['identifier'];
      $exposed = $this->view->get_exposed_input();
      if (isset($exposed[$id]) && empty($exposed[$id])) {
        $section = oa_core_get_section_context();
        if (($node = menu_get_object()) && $node->type == 'oa_space') {
          $section = NULL;
        }
        if ($section) {
          $exposed[$id] = $section;
        }
        else {
          unset($exposed[$id]);
        }
        $this->view->set_exposed_input($exposed);
      }
    }
    $exposed = $this->view->get_exposed_input();
    // Remove section value if seciton not in group.
    if (!empty($exposed[$id]) && is_numeric($exposed[$id]) && !empty($exposed['og_group_ref_target_id']) && ($section = node_load($exposed[$id])) && $section->og_group_ref[LANGUAGE_NONE][0]['target_id'] != $exposed['og_group_ref_target_id']) {
      unset($exposed[$id]);
      $this->view->set_exposed_input($exposed);
    }
  }

  function exposed_form(&$form, &$form_state) {
    parent::exposed_form($form, $form_state);
    if (!empty($form_state['input'][$id]) && !in_array($form_state['input'][$id], $form[$id]['#options'])) {
      unset($form_state['input'][$id]);
    }
  }
}