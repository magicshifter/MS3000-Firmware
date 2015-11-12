(ns magic-ring.components.clock
  (:require-macros [cljs.core.async.macros :refer [go]])
  (:require [goog.dom :as gdom]
            [om.next :as om :refer-macros [defui]]
            [sablono.core :as html :refer-macros [html]]
            [cljs-http.client :as http]
            [cljs.core.async :refer [<!]]
            [magic-ring.appstate :as app]))

(enable-console-print!)

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

(defmethod mutate 'time/interval
  [{:keys [state]} k {:keys [interval]}]
  {:action (fn []
             (swap! state assoc :interval interval))})

(def reconciler
  (om/reconciler
    {:state app/state
     :parser (om/parser {:read read :mutate mutate})}))

(defn post-time [owner cb]
  "send the current time to the device"
  (let [{:keys [settings time]} (om/props owner)
        {:keys [host protocol timeout]} settings
         post-url (str protocol "://" host "/time")]
    (js/console.log "posting to url" post-url)
    (go
      (let [response (<! (http/post post-url
                             {:with-credentials? false
                              :timeout timeout
                              :query-params {"t" time}}))]
        (cb (-> response :error-text) response)))))

(defn post-time-cb [err response]
  "called after a post-request. TODO: get a notification to pop up or somethin'"
  (if (= "" err)
    (js/console.log "time set: " response)
    (js/console.error err response)))

(defn swap-time [owner]
  "reset the app/state time every ~second"
  (let [time (int (/ (.getTime (js/Date.)) 1000))]
    (om/transact! owner `[(time/update {:time ~time})])))

(defui Component
  static om/IQuery
  (query [this]
    [:time :settings :interval])

  Object
  (componentWillMount [this]
    "add interval that counts up the time every second"
    (let [interval (js/setInterval #(swap-time this) 1000)]
      (om/transact! this `[(time/interval {:interval ~interval})])))

  (componentWillUnmount [this]
    "remove the time interval on unmount"
    (let [{:keys [interval]} (om/props this)]
      (js/clearInterval interval)
      (om/transact! this `[(time/interval {:interval nil})])))

  (render [this]
    (let [{:keys [time]} (om/props this)]
      (html
        [:div
         [:span (str "Current Time: " time)]
         [:input {:type "submit"
                  :value "Set Clock"
                  :onClick #(post-time this post-time-cb)}]]))))

(def component (om/factory Component))

(defn get-or-create-dom [id parent]
  (if-let [ele (gdom/getElement id)]
    ele
    (let [ele (gdom/createElement "div")]
      (gdom/appendChild parent ele)
      (gdom/setProperties ele {:id id})
      ele)))

(defn init []
  (let [app-ele (gdom/getElement "app")
        ele (get-or-create-dom "clock" app-ele)]
    (om/add-root! reconciler Component (gdom/getElement ele))))