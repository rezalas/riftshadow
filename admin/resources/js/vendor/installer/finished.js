import "../../bootstrap";
import { createApp } from "vue";
import queueProgress from "./components/queue-progress.vue";

createApp({
	data() {
		return {
			canExit: false,
		};
	},
	components: {
		queueProgress,
	},
	mounted() {
		window.addEventListener("beforeunload", (evt) => {
			if (!this.canExit) {
				evt.preventDefault();
				evt.returnValue = "";
			}
		});
		this.$on("can-exit", () => {
			this.canExit = true;
		});
	},
}).mount("#finished");
