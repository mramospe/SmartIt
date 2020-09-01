#!/usr/bin/env python
"""Make the timing plots
"""
import matplotlib.pyplot as plt
import numpy as np

if __name__ == '__main__':

    fig, (ax0, ax1) = plt.subplots(1, 2, figsize=(12, 7))

    for dt, ax in (('array', ax0), ('vector', ax1)):

        entries, std, var_std, smit, var_smit = np.loadtxt(
            f'{dt}.txt', dtype=np.float32, delimiter=' ').T

        print(f'Values for {dt}')
        for r in zip(entries, std, var_std, smit, var_smit):
            print(*r)

        ax.set_title(dt)
        ax.errorbar(entries, 1e3 * smit, yerr=np.sqrt(var_smit),
                    color='C1', marker='*', ls='-', ms=10, lw=1.5, label='smartit')
        ax.errorbar(entries, 1e3 * std, yerr=np.sqrt(var_smit),
                    color='C0', marker='^', ls='--', ms=10, lw=1.5, label='std')
        ax.set_xlabel('entries')
        ax.set_xscale('log', nonposx='clip')
        ax.set_ylabel('time (ms)', ha='right', y=1.)
        ax.legend(loc='upper left')

    fig.tight_layout()

    plt.show()
