const binding = require(`./build/Release/gc_load`);

class GCLoad {
    lastTime = process.hrtime();
    lastTotal = 0;
    binding = null;
    start() {
        if (!this.binding) {
            this.binding = new binding.GCLoad();
            this.binding.start();
        }
    }
    stop() {
        if (this.binding) {
            this.binding.stop();
            this.binding = null;
        }
    }
    load() {
        if (this.binding) {
            const { lastTime, lastTotal } = this;
            const now = process.hrtime();
            const total = this.binding.total();
            this.lastTime = now;
            this.lastTotal = total;
            const cost = total - lastTotal;
            const interval = (now[0] - lastTime[0]) * 1e6 + (now[1] - lastTime[1]) / 1e3;
            return cost / interval;
        }
    }
    total() {
        if (this.binding) {
            return this.binding.total();
        }
    }
}

module.exports = {
    GCLoad
}