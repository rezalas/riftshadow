import "../../bootstrap";
import Vue from "vue";
import queueProgress from "./components/queue-progress";

new Vue({
	el: "#finished",
	data: {
		canExit: false
	},
	components: {
		queueProgress
	},
	mounted() {
		window.addEventListener("beforeunload", evt => {
			if (!this.canExit) {
				evt.preventDefault();
				evt.returnValue = "";
			}
		});
		this.$on("can-exit", () => {
			this.canExit = true;
		});
	}
});
