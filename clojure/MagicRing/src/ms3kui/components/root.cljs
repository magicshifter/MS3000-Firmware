(ns ms3kui.components.root
  (:require
    [om.next :as om :refer-macros [defui]]
    [goog.dom :as gdom]
    [sablono.core :as html :refer-macros [html]]
    [ms3kui.appstate :as app]
    [ms3kui.components.clock :as clock]))

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
    [:time :settings])
  Object
  (render [this]
    (html
      [:div.pages
       (let [{:keys [time settings]} (om/props this)]
         (clock/component {:time time
                           :owner this
                           :settings (-> settings :client)}))])))

(defn init []
  (om/add-root! reconciler clock/Component (gdom/getElement "app")))