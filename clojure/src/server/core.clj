(ns server.core
  (:require [compojure.api.sweet :refer :all]
            [compojure.handler :as handler]
            [compojure.route :as route]
            [ring.util.http-response :refer :all]
            [schema.core :as s]
            [ring.middleware.cors :as cors]))

(s/defschema Message {:message String})

(s/defschema Settings {:leds s/Int})

(def led-count 16)

(defroutes* app-routes
  (GET* "/" [value]
    (ok {:value value}))
  (route/resources "")
  (route/not-found "Not Found"))

(defn allow-cross-origin
  "middleware function to allow crosss origin"
  [handler]
  (fn [request]
    (let [response (handler request)]
      (assoc-in response [:headers "Access-Control-Allow-Origin"] "*"))))

(defn options-200
  "middleware function to always 200 an OPTIONS request"
  [handler]
  (fn [request]
    (if (= :options (:request-method request))
      {:headers {"Access-Control-Allow-Origin" "*"
                 "Access-Control-Allow-Methods" "GET, POST, PUT, OPTIONS"}
       :body ""
       :status 204}
      (handler request))))


(defapi app
  (-> (handler/site app-routes)
    (cors/wrap-cors :access-control-allow-origin #".*"
      :access-control-allow-credentials "true"
      :access-control-allow-headers ["Origin"
                                     "X-Requested-With"
                                     "Content-Type"
                                     "Accept"]
      :access-control-allow-methods ["GET"
                                     "POST"
                                     "PUT"
                                     "DELETE"
                                     "OPTIONS"])
    (options-200)
    (allow-cross-origin)
    ))

;(defapi app
;  (swagger-ui)
;  (swagger-docs
;   {:info {:title "Api"
;           :description "MagicShifter 3000 Api Mock"}
;    :tags [{:name "settings",
;            :description "returns MagicShifter 3000 settings"}]})
;  (context* "/settings" []
;            :tags ["settings"]
;            (wrap-cors :access-control-allow-origin #".+" :access-control-allow-methods [:get :put :post :delete]
;                       (GET* "/" []
;                             :return Settings
;                            :summary "get ms3000 settings"
;                             (ok {:leds (-> led-count)})))))
