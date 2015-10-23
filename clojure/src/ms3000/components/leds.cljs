(ns ms3000.components.leds
  (:require
    [om.core :as om :include-macros true]
    [om.dom :as dom :include-macros true]
    [sablono.core :as html :refer-macros [html]]
    [ms3000.components.led :as led]))

(defn- create-matrix [app]
  (let [rows (-> app :editor :rows) cols (-> app :editor :cols)]
    (for [col (range cols)]
      [:li.col {:key col}
       [:ul
        (for [row (range rows)]
          (om/build led/component app {:init-state {:row row :col col}}))]])))

(defn- find-led [app row col]
  (let [leds (-> app :leds)]
    (for [led (range leds)]
      (println led))))

(defn- init [app]
  []
  (let [rows (-> app :editor :rows) cols (-> app :editor :cols) leds []]
    (do
      (for [row (range rows)]
        (for [col (range cols)]
          (conj leds {:r 255 :g 255 :b 255 :a 255
                      :active false
                      :row row
                      :col col}))))))

(defn component [app owner]
  (reify
    om/IInitState
      (init-state [_] (init app))

    om/IRenderState
    (render-state [_ state]
      (println (find-led app 0 0))
      (html
        [:ul.leds
         (create-matrix app)]))))
