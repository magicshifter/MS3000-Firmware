(ns ms3000.components.leds
  (:require-macros [cljs.core.async.macros :refer [go]])
  (:require
   [om.core :as om :include-macros true]
   [om.dom :as omdom :include-macros true]
   [cljs.core.async :refer [put! chan <! alts!]]
   [sablono.core :as html :refer-macros [html]]
   [ms3000.components.led :as led]))

(defn component [data]
  (om/component
   (html
    [:ul.leds {:list-style "none"}
     (for [led-item (-> data :leds)]
       (led/component data led-item))])))
