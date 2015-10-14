(defproject ms3000 "0.1.0-SNAPSHOT"
  :description "A prototype for the MagicShifter3000 UserInterface, written in ClojureScript"
  :url "http://github.com/magicshifter/onboard-ui"
  :license {:name "AGPL v3.0"}

  :dependencies [[org.clojure/clojure "1.7.0"]
                 [org.clojure/clojurescript "1.7.122"]
                 [org.clojure/core.async "0.1.346.0-17112a-alpha"]
                 [sablono "0.3.6"]
                 [org.omcljs/om "0.9.0"]]

  :plugins [[lein-cljsbuild "1.1.0"]
            [lein-figwheel "0.4.1"]]

  :source-paths ["src"]

  :clean-targets ^{:protect false} ["resources/public/js/compiled" "target" "resources/public/css/bundle.css"]

  :cljsbuild {:builds [ {:id "app"
                         :source-paths ["src"]
                         :figwheel {:on-jsload "ms3000.core/on-js-reload"}
                         :compiler {:main ms3000.core
                                    :asset-path "js/compiled/out"
                                    :output-to "resources/public/js/compiled/ui.js"
                                    :output-dir "resources/public/js/compiled/out"
                                    :source-map-timestamp true}}
                        {:id "min"
                         :source-paths ["src"]

                         :compiler {:output-to "resources/public/js/compiled/ui.js"
                                    :main ms3000.core
                                    :optimizations :advanced
                                    :pretty-print false}}]}

  :figwheel {
             ;; :http-server-root "public" ;; default and assumes "resources"
             ;; :server-port 3449 ;; default
             ;; :server-ip "127.0.0.1"

             :css-dirs ["resources/public/css"] ;; watch and update CSS

             ;; Start an nREPL server into the running figwheel process
             ;; :nrepl-port 7888
             })
