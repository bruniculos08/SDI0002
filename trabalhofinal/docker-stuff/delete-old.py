from cassandra.cluster import Cluster
import time
import random
import numpy as np
from matplotlib import pyplot as plt

cluster = Cluster()
cluster = Cluster(['172.17.0.2','172.17.0.3','172.17.0.4'], port=9042)

session = cluster.connect('sensores') 
session.set_keyspace('sensores')

session.execute('USE sensores')

current_rows = session.execute('SELECT COUNT(*) FROM temperaturas WHERE momento_sensor >= minTimeuuid(to_timestamp(to_date(now()))) ALLOW FILTERING');
n = session.execute('SELECT COUNT(*) FROM sensores.temperaturas')

print("Número de linhas (com data do dia atual) na tabela 'sensores.temperaturas': " + str(current_rows[0][0]))
print("Número de linhas na tabela 'sensores.temperaturas': " + str(n[0][0]))
