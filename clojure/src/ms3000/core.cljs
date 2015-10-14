(ns ^:figwheel-always ms3000.core
  (:require
    [om.core :as om :include-macros true]
    [om.dom :as dom :include-macros true]
    [ms3000.appstate :as app]
    [ms3000.components.leds :as leds]
  )
)

(enable-console-print!)

(println "Welcome to the MagicShifter 3000 App.")

(om/root leds/list app/app-state {:target js/document.body})

(defn on-js-reload []
  ;; optionally touch the app-state to force rerendering
)
