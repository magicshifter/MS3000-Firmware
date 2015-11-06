(defproject ms3kui "0.0.1-SNAPSHOT"
  :description "MagicShifter 3000 User Interface"
  :author "Wizards@Work"
  :license {:name "Affero General Public License"
            :url "https://gnu.org/licenses/agpl.html"
            :distribution "https://github.com/hackerspaceshop/MagicShifter3000"
            :comments "cooperate, innovate, celebrate"}

  :dependencies [[org.clojure/clojure "1.7.0"]
                 [org.clojure/clojurescript "1.7.145"]
                 [org.omcljs/om "1.0.0-alpha9"]
                 [figwheel-sidecar "0.4.1" :scope "provided"]
                 [sablono "0.3.6"]
                 [cljs-http "0.1.37"]
                 [hiccup "1.0.5"]
                 [medley "0.7.0"]
                 [prismatic/schema "1.0.3"]]

  :plugins [[hiccup-watch "0.1.1"]
            [lein-garden "0.2.6"]
            [lein-cljsbuild "1.1.0"]]

  :hiccup-watch {:input-dir "resources/hiccup"
                 :output-dir "resources/public"}

  :aliases {"dev" ["run" "-m" "clojure.main" "script/figwheel.clj"]}

  :garden {:builds [{:id "dev"
                     ;; Source paths where the stylesheet source code is
                     :source-paths ["resources/css"]
                     ;; The var containing your stylesheet:
                     :stylesheet css.core/screen
                     ;; Compiler flags passed to `garden.core/css`:
                     :compiler {;; Where to save the file:
                                :output-to "resources/public/css/screen.css"
                                ;; Compress the output?
                                :pretty-print? true}}

                    {:id "min"
                     :source-paths ["resources/css"]
                     :stylesheet css.core/screen
                     :compiler {:output-to "resources/public/css/screen.css"
                                :pretty-print? false}}]}

  :cljsbuild { :builds [{:id "min"
                         :source-paths ["src"]

                         :compiler {:output-to "resources/public/js/main.min.js"
                                    :main ms3kui.core
                                    :optimizations :advanced
                                    :pretty-print false}}]}

  :clean-targets ^{:protect false} ["resources/public/js"
                                    "resources/public/css"
                                    "resources/public/index.html"
                                    "figwheel_server.log"
                                    "target"])
