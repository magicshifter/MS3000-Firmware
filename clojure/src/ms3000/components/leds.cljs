(ns ms3000.components.leds
  (:require-macros [cljs.core.async.macros :refer [go]])
  (:require
   [om.core :as om :include-macros true]
   [om.dom :as omdom :include-macros true]
   [cljs.core.async :refer [put! chan <! alts!]]
   [sablono.core :as html :refer-macros [html]]))

(defn on-click [data led event]
  (print (-> led :id))
  (update-in data [:leds] led))

(defn led-single [data led]
  [:li
   {:key (str 'led- (-> led :id))
    :style {:float "left"
            :display "inline-block"
            :border "1px solid"}}
   [:div {:style {:background-color (-> led :color)
                  :height "20px"
                  :width "20px"}
          :on-click #(on-click data led %1)}]])

(defn led-list [data]
  [:ul
   {:list-style "none"}
   (for [led (-> data :leds)]
     (led-single data led))])

(defn list [data]
  (om/component
   (html
    [:div
     [:h1 (-> data :title)]
     [:div
      (led-list data)]])))
