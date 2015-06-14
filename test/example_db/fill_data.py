#!/usr/bin/python


import argparse
import random
import ipaddr
import sqlite3
from collections import namedtuple
from datetime import timedelta, datetime


CLIENTS = "10.10.10.0/24" # CIDR
INTERVAL = 24*7 # hours
STEP = 10 # minutes



class TimeInterval(object):
    _epoch = datetime(1970,1,1)

    def __init__(self, interval, step):
        if not isinstance(interval, timedelta):
            interval = timedelta(hours=interval)
        if not isinstance(step, timedelta):
            step = timedelta(minutes=step)

        self.interval = interval
        self.step = step
        self.now = datetime.now()

    def __iter__(self):
        stop = self.now - self.interval
        while stop < self.now:
            yield int((stop - self._epoch).total_seconds())
            stop += self.step

class ClientBytes(object):
    def __init__(self):
        self.lastbytes = random.randint(0, 1024*1024*1024*3)

    def nextbytes(self):
        newbytes = max(0, self.lastbytes + random.randint(-1024*1024*500, 1024*1024*500))
        self.lastbytes = newbytes
        return newbytes
        

Record = namedtuple("Record", ["ip", "input", "output", "timestamp"])
Client = namedtuple("Client", ["input", "output", "ip"])


def create_data(clients, times):
    data = []

    for timestamp in times:
        data.extend([Record(ip=c.ip, input=c.input.nextbytes(), output=c.output.nextbytes(), timestamp=timestamp) for c in clients])

    return data


def write_data(connection, data):
    input = "INSERT INTO inbound (ip_dst, bytes, stamp_inserted, stamp_updated) VALUES (?, ?, ?, ?)"
    output = "INSERT INTO outbound (ip_src, bytes, stamp_inserted, stamp_updated) VALUES (?, ?, ?, ?)"

    connection.executemany(input, [(r.ip, r.input, r.timestamp, r.timestamp) for r in data])
    connection.executemany(output, [(r.ip, r.output, r.timestamp, r.timestamp) for r in data])

    connection.commit()


parser = argparse.ArgumentParser(description='Insert example data.')

parser.add_argument('clients', metavar='C', type=str,
                    help='CIDR describing the clients')
parser.add_argument("--interval", type=int, help="Interval to generate data for in HOURS")
parser.add_argument("--step", type=int, help="Step within the interval in MINUTES")

if __name__ == "__main__":
    args = parser.parse_args()

    clients = [Client(ip=str(ip), input=ClientBytes(), output=ClientBytes()) for ip in ipaddr.IPNetwork(args.clients).iterhosts()]

    connection = sqlite3.connect('example.db')
    data = create_data(clients, TimeInterval(args.interval, args.step))
    write_data(connection, data)
