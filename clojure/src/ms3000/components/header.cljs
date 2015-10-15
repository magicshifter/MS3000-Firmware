(ns ms3000.components.header
  (:require
   [om.core :as om :include-macros true]
   [om.dom :as omdom :include-macros true]
   [sablono.core :as html :refer-macros [html]]))

(defn component [data]
  (om/component
   (html
    [:div
     [:h1 (-> data :title)]])))
