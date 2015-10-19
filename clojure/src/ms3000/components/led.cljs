(ns ms3000.components.led
  (:require
    [om.core :as om :include-macros true]
    [om.dom :as omdom :include-macros true]
    [sablono.core :as html :refer-macros [html]]))

(defn- toggle-boolean [bool]
  (= bool false))

(defn- on-click [app led event]
  (let [leds (-> app :leds)]
    (println "led clicked" (-> led :col) (-> led :row))))

(defn- calculate-border-color [is-active?]
  (if (= true is-active?)
    "#ffffff"
    "#000000"))

(defn- init-component [app _]
  (let [led {:r 255 :g 255 :b 255 :a 255 :active false}]
    (om/transact! app [:leds] (fn [_] (conj (-> app :leds) led)))
    led))

(defn- render-component [app led]
  (html
    (let [id (str "cell-" (-> led :row) "-" (-> led :col))
          r (-> led :r)
          g (-> led :g)
          b (-> led :b)
          a (-> led :a)]
      [:li.led {:on-click #(on-click app led %1)
                :style {:background-color (str "rgba(" r "," g "," b "," a ")")
                        :border-color (calculate-border-color (-> led :active))}}])))

(defn component [app led-opts]
  (let [row (-> led-opts :opts :row) col (-> led-opts :opts :col)]
    (reify
      om/IInitState
      (init-state [_] (init-component app led-opts))
      om/IRenderState
      (render-state [_ state] (render-component app state))
      om/IWillUnmount
      (will-unmount [_] (println "will unmount")))))
