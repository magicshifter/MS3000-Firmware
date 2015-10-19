(ns ms3000.components.controls
  (:require-macros [cljs.core.async.macros :refer [go]])
  (:require
    [om.core :as om :include-macros true]
    [cljs.core.async :refer [put! chan <! alts!]]
    [sablono.core :as html :refer-macros [html]]))

(defn handle-change [event data]
  (let [val (max 1 (.. event -target -value))]
    (om/transact! data [:editor :cols]
      (fn [] val))))

(defn component [data owner]
  (om/component
    (html
      [:ul.controls
       [:li
        [:label "Columns:"
         [:input {:type "number"
                  :min 1
                  :value (-> data :editor :cols)
                  :onChange #(handle-change % data)}]]]])))
