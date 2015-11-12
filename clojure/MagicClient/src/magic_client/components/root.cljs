(ns magic-client.components.root
  (:require
    [om.next :as om :refer-macros [defui]]
    [goog.dom :as gdom]
    [sablono.core :as html :refer-macros [html]]
    [magic-client.appstate :as app]))

(defn read [{:keys [state] :as env} key params]
  (let [st @state]
    (if-let [[_ value] (find st key)]
      {:value value}
      {:value :not-found})))

(defmulti mutate om/dispatch)

(defmethod mutate 'time/update
  [{:keys [state]} k {:keys [time]}]
  {:action (fn []
             (swap! state assoc :time time))})

(def reconciler
  (om/reconciler
    {:state app/state
     :parser (om/parser {:read read :mutate mutate})}))

(defui component
  static om/IQuery
  (query [this]
    [:settings])
  Object
  (render [this]
    (html
      [:div.pages (let [settings (om/props this)]
                    [:div (str settings)])])))

(defn init []
  (om/add-root! reconciler component (gdom/getElement "app")))