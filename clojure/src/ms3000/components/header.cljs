(ns ms3000.components.header
  (:require
    [om.core :as om :include-macros true]
    [sablono.core :as html :refer-macros [html]]))

(defn component [state owner]
  (om/component
    (html
      [:div
       [:h1 (-> state :title)]])))
