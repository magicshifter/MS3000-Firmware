(ns ms3kui.components.clock
  (:require-macros [cljs.core.async.macros :refer [go]])
  (:require [goog.dom :as gdom]
            [om.next :as om :refer-macros [defui]]
            [sablono.core :as html :refer-macros [html]]
            [cljs-http.client :as http]
            [cljs.core.async :refer [<!]]
            [ms3kui.appstate :as app]))

(enable-console-print!)

;; The http requests to the MagicRing
(defn post-time [owner cb]
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
  (if (= "" err)
    (js/console.log "time set: " response)
    (js/console.error err response)))

(defn swap-time [owner]
  (let [time (int (/ (.getTime (js/Date.)) 1000))]
    (om/transact! owner `[(time/update {:time ~time})])))

(defui Component
  static om/IQuery
  (query [this]
    [:time :settings])

  Object
  (componentWillMount [this]
    (let [interval (js/setInterval #(swap-time this) 1000)]
      ;(js/clearInterval interval)
      (js/console.log "interval" interval)))

  (componentWillUnmount [this]
    (js/console.log "unmount now"))

  (render [this]
    (let [{:keys [time]} (om/props this)]
      (html
        [:div
         [:span (str "Current Time: " time)]
         [:input {:type "submit"
                  :value "Set Clock"
                  :onClick #(post-time this post-time-cb)}]]))))

(def component (om/factory Component))