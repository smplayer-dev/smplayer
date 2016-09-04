#if USE_PULSEAUDIO_DEVICES
#include <pulse/context.h>
#include <pulse/glib-mainloop.h>
#include <pulse/introspect.h>
#include <pulse/operation.h>
#include <QApplication>

QMap<int, QString> pulse_device_list;
bool pulse_list_done = false;

void sink_cb(pa_context *, const pa_sink_info *i, int eol, void *) {
	if (eol == 0) {
		qDebug() << "DeviceInfo::sink_cb: sink #" << i->index << ":" << i->name;
		pulse_device_list[i->index] = i->name;
	} else {
		pulse_list_done = true;
	}
}

void context_state_cb(pa_context *c, void *) {
	qDebug() << "DeviceInfo::context_state_cb";

	pa_context_state_t state = pa_context_get_state(c);

	if (state == PA_CONTEXT_READY) {
		pa_operation *o;

		if (!(o = pa_context_get_sink_info_list(c, sink_cb, NULL))) {
			qWarning() << "DeviceInfo::context_state_cb: pa_context_get_sink_info_list failed";
			return;
		}
		pa_operation_unref(o);
	}
}

DeviceList DeviceInfo::paDevices() {
	qDebug("DeviceInfo::paDevices");
	DeviceList l;

	if (!pulse_list_done) {
		pa_glib_mainloop *mainloop = pa_glib_mainloop_new(NULL);
		pa_mainloop_api *api = pa_glib_mainloop_get_api(mainloop);
		pa_context *context = pa_context_new(api, "smplayer");

		if (pa_context_connect(context, NULL, PA_CONTEXT_NOFAIL, 0) < 0) {
			pa_context_unref(context);
			return l;
		}
		pa_context_set_state_callback(context, context_state_cb, NULL);

		// Wait for a while until the list is complete
		for (int n = 0; n < 1000; n++) {
			QApplication::processEvents();
			if (pulse_list_done) {
				qDebug("DeviceInfo::paDevices: list done (%d)", n);
				break;
			}
		}
	}

	if (!pulse_list_done) {
		qWarning("DeviceInfo::paDevices: list is not done yet!");
	}

	QMapIterator<int, QString> i(pulse_device_list);
	while (i.hasNext()) {
		i.next();
		l.append(DeviceData(i.key(), i.value()));
	}

	return l;
}
#endif // USE_PULSEAUDIO_DEVICES
