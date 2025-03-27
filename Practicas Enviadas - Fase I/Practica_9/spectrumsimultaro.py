#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: Not titled yet
# Author: hmro2
# GNU Radio version: 3.10.10.0

from PyQt5 import Qt
from gnuradio import qtgui
from gnuradio import blocks
from gnuradio import fft
from gnuradio.fft import window
from gnuradio import gr
from gnuradio.filter import firdes
import sys
import signal
from PyQt5 import Qt
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
import sip
import spectrumsimultaro_epy_block_0 as epy_block_0  # embedded python block



class spectrumsimultaro(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Not titled yet", catch_exceptions=True)
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Not titled yet")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except BaseException as exc:
            print(f"Qt GUI: Could not set Icon: {str(exc)}", file=sys.stderr)
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "spectrumsimultaro")

        try:
            geometry = self.settings.value("geometry")
            if geometry:
                self.restoreGeometry(geometry)
        except BaseException as exc:
            print(f"Qt GUI: Could not restore geometry: {str(exc)}", file=sys.stderr)

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 44000
        self.longitud = longitud = -73.12083990526662
        self.latitud = latitud = 7.136921524265512
        self.N = N = 8

        ##################################################
        # Blocks
        ##################################################

        self.qtgui_vector_sink_f_0 = qtgui.vector_sink_f(
            N,
            0,
            1.0,
            "x-Axis",
            "y-Axis",
            "",
            1, # Number of inputs
            None # parent
        )
        self.qtgui_vector_sink_f_0.set_update_time(0.10)
        self.qtgui_vector_sink_f_0.set_y_axis((-140), 10)
        self.qtgui_vector_sink_f_0.enable_autoscale(True)
        self.qtgui_vector_sink_f_0.enable_grid(False)
        self.qtgui_vector_sink_f_0.set_x_axis_units("")
        self.qtgui_vector_sink_f_0.set_y_axis_units("")
        self.qtgui_vector_sink_f_0.set_ref_level(0)


        labels = ['', '', '', '', '',
            '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
            "magenta", "yellow", "dark red", "dark green", "dark blue"]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]

        for i in range(1):
            if len(labels[i]) == 0:
                self.qtgui_vector_sink_f_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_vector_sink_f_0.set_line_label(i, labels[i])
            self.qtgui_vector_sink_f_0.set_line_width(i, widths[i])
            self.qtgui_vector_sink_f_0.set_line_color(i, colors[i])
            self.qtgui_vector_sink_f_0.set_line_alpha(i, alphas[i])

        self._qtgui_vector_sink_f_0_win = sip.wrapinstance(self.qtgui_vector_sink_f_0.qwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_vector_sink_f_0_win)
        self.qtgui_sink_x_0 = qtgui.sink_f(
            1024, #fftsize
            window.WIN_BLACKMAN_hARRIS, #wintype
            0, #fc
            samp_rate, #bw
            "", #name
            True, #plotfreq
            True, #plotwaterfall
            True, #plottime
            True, #plotconst
            None # parent
        )
        self.qtgui_sink_x_0.set_update_time(1.0/10)
        self._qtgui_sink_x_0_win = sip.wrapinstance(self.qtgui_sink_x_0.qwidget(), Qt.QWidget)

        self.qtgui_sink_x_0.enable_rf_freq(False)

        self.top_layout.addWidget(self._qtgui_sink_x_0_win)
        self.fft_vxx_0 = fft.fft_vfc(N, True, window.blackmanharris(N), True, 1)
        self.epy_block_0 = epy_block_0.blk(url_google_sheet='https://docs.google.com/spreadsheets/d/1ZI1pO5DJjVnh0ph0jH8sRjnLGqXHOz-jBXJ1ZF875JM/edit?usp=sharing', latitud=latitud, longitud=longitud, N=N, finicial=1417e6, fpaso=1000, salto=40000)
        self.blocks_wavfile_source_0 = blocks.wavfile_source('D:\\INGENIERIA ELECTRONICA\\12. Doceavo Semestre\\IoT\\Practica_9\\voz_grabada.wav', True)
        self.blocks_throttle2_0 = blocks.throttle( gr.sizeof_float*1, samp_rate, True, 0 if "auto" == "auto" else max( int(float(0.1) * samp_rate) if "auto" == "time" else int(0.1), 1) )
        self.blocks_stream_to_vector_0 = blocks.stream_to_vector(gr.sizeof_float*1, N)
        self.blocks_complex_to_mag_0 = blocks.complex_to_mag(N)


        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_complex_to_mag_0, 0), (self.epy_block_0, 0))
        self.connect((self.blocks_complex_to_mag_0, 0), (self.qtgui_vector_sink_f_0, 0))
        self.connect((self.blocks_stream_to_vector_0, 0), (self.fft_vxx_0, 0))
        self.connect((self.blocks_throttle2_0, 0), (self.blocks_stream_to_vector_0, 0))
        self.connect((self.blocks_wavfile_source_0, 0), (self.blocks_throttle2_0, 0))
        self.connect((self.blocks_wavfile_source_0, 0), (self.qtgui_sink_x_0, 0))
        self.connect((self.fft_vxx_0, 0), (self.blocks_complex_to_mag_0, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "spectrumsimultaro")
        self.settings.setValue("geometry", self.saveGeometry())
        self.stop()
        self.wait()

        event.accept()

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle2_0.set_sample_rate(self.samp_rate)
        self.qtgui_sink_x_0.set_frequency_range(0, self.samp_rate)

    def get_longitud(self):
        return self.longitud

    def set_longitud(self, longitud):
        self.longitud = longitud
        self.epy_block_0.longitud = self.longitud

    def get_latitud(self):
        return self.latitud

    def set_latitud(self, latitud):
        self.latitud = latitud
        self.epy_block_0.latitud = self.latitud

    def get_N(self):
        return self.N

    def set_N(self, N):
        self.N = N
        self.epy_block_0.N = self.N




def main(top_block_cls=spectrumsimultaro, options=None):

    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()

    tb.start()

    tb.show()

    def sig_handler(sig=None, frame=None):
        tb.stop()
        tb.wait()

        Qt.QApplication.quit()

    signal.signal(signal.SIGINT, sig_handler)
    signal.signal(signal.SIGTERM, sig_handler)

    timer = Qt.QTimer()
    timer.start(500)
    timer.timeout.connect(lambda: None)

    qapp.exec_()

if __name__ == '__main__':
    main()
