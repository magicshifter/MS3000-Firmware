(ns ms3000.components.led
  (:require-macros [cljs.core.async.macros :refer [go]])
  (:require
   [om.core :as om :include-macros true]
   [om.dom :as omdom :include-macros true]
   [cljs.core.async :refer [put! chan <! alts!]]
   [sablono.core :as html :refer-macros [html]]))

(defn on-click [data led-item event]
  (println "clicked led item:" led-item))

(defn component [data led-item]
   (html
    [:li.led {:key (str 'led- (-> led-item :id))
              :on-click #(on-click data led-item %1)
              :style {:background-color (-> led-item :color)}}]))
