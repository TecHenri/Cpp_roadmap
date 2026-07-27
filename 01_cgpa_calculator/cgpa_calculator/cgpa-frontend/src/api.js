// Toutes les fonctions d'appel à l'API C++ backend
// Centralisées ici pour ne jamais dupliquer l'URL de base

import axios from 'axios';

const BASE = 'http://localhost:8080/api';

export const api = {
  getStudents:  ()         => axios.get(`${BASE}/students`),
  getStudent:   (id)       => axios.get(`${BASE}/students/${id}`),
  addStudent:   (data)     => axios.post(`${BASE}/students`, data),
  deleteStudent:(id)       => axios.delete(`${BASE}/students/${id}`),
  sortStudents: (by)       => axios.get(`${BASE}/students/sort?by=${by}`),
  searchStudent:(id)       => axios.get(`${BASE}/students/search?id=${id}`),
  getStats:     ()         => axios.get(`${BASE}/stats`),
};
