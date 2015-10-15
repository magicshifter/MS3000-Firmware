(ns ms3000.components.led
  (:require-macros [cljs.core.async.macros :refer [go]])
  (:require
   [om.core :as om :include-macros true]
   [om.dom :as omdom :include-macros true]
   [cljs.core.async :refer [put! chan <! alts!]]
   [sablono.core :as html :refer-macros [html]]))

(defn toggle-boolean [bool]
  (= bool false))

(defn on-click [data id event]
  (om/transact! data [:leds id :active] toggle-boolean))

(defn calculate-border-color [is-active?]
  (if (= true is-active?)
    "#ffffff"
    "#000000"))

(defn component [data led-item]
  (html
   [:li.led {:key (str (-> led-item :id))
             :on-click #(on-click data (-> led-item :id) %1)
             :style {:background-color (-> led-item :color)
                     :border-color (calculate-border-color (-> led-item :active))}}]))
