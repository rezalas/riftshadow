<style lang="scss" scoped>
// Any custom styling goes here
</style>

<template>
	<div>
		<div v-show="cur > jobCount && cur <= max">
			Estimated time left: {{ time }}
		</div>
		<progress class="installer-progress-bar" :value="cur" :max="max">
		</progress>
	</div>
</template>

<script>
import axios from "axios";
import moment from "moment";

export default {
	data() {
		return {
			cur: 0, // The current task number
			data: {}, // Additional data to send to the endpoint
			jobCount: 0, // The amount of jobs
			max: 0, // The max task number
			lastSecs: [] // The seconds it takes to process the jobs
		};
	},
	computed: {
		/**
		 * Get the total seconds left in jobs
		 *
		 * @returns {Number}
		 */
		jobTotalSeconds() {
			// lastSecs is sometimes an object
			return Object.keys(this.lastSecs).reduce(
				(total, key) => total + this.lastSecs[key],
				0
			);
		},
		/**
		 * Get a human readable representation of the time left
		 * to process the queue
		 *
		 * @returns {String}
		 */
		time() {
			let secTotal = this.max * this.jobTotalSeconds,
				secProcessed = this.cur * this.jobTotalSeconds,
				mSecs = (secTotal - secProcessed).toFixed(2) * 1000;
			return moment.duration(mSecs).humanize();
		}
	},
	methods: {
		/**
		 * Update the queue progress and process the queue
		 *
		 * @param {Boolean} hasStarted Whether or not process has started
		 */
		updateProgress(hasStarted = false) {
			axios
				.post("/install/database/process-db-imports", {
					cur: this.cur,
					max: this.max,
					hasStarted,
					jobCount: this.jobCount,
					lastSecs: this.data.lastSecs
				})
				.then(({ data, statusText }) => {
					let { cont, cur, jobCount, lastSecs, max } = data,
						erred = false;

					this.data = data;
					this.jobCount = jobCount;

					if (!hasStarted) {
						// Only set max if it hasn't started
						this.max = max;
					}

					this.cur = cur;
					this.lastSecs = lastSecs;

					if (statusText.indexOf("Failed") >= 0) {
						cont = false;
						erred = true;
					}

					if (cont) {
						this.updateProgress(true);
					} else if (!cont && !erred) {
						this.$root.$emit("can-exit");
					}
				});
		}
	},
	mounted() {
		this.updateProgress();
	}
};
</script>
