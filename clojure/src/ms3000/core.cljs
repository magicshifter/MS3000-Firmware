(ns ^:figwheel-always ms3000.core
  (:require [om.core :as om :include-macros true]
            [om.dom :as dom :include-macros true]
            [ms3000.appstate :as app]
            [ms3000.components.leds :as leds]
            [ms3000.components.header :as header]))

(enable-console-print!)

(println "Welcome to the MagicShifter 3000 App.")

(om/root header/component app/app-state {:target (. js/document (querySelector "header.main"))})

(om/root leds/component app/app-state {:target (. js/document (querySelector "#app"))})

(defn on-js-reload []
  ;; optionally touch the app-state to force rerendering
  )
