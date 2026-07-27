import { useEffect, useState } from 'react';
import { api } from '../api';

export default function Students() {
  const [students,  setStudents]  = useState([]);
  const [sortBy,    setSortBy]    = useState('cgpa');
  const [selected,  setSelected]  = useState(null);
  const [error,     setError]     = useState('');
  const [msg,       setMsg]       = useState('');

  const load = (by = sortBy) => {
    api.sortStudents(by).then(r => setStudents(r.data)).catch(() => setError('Erreur de chargement'));
  };

  useEffect(() => { load(); }, []);

  const handleSort = (by) => { setSortBy(by); load(by); };

  const handleDelete = (id) => {
    if (!window.confirm(`Supprimer l'étudiant ${id} ?`)) return;
    api.deleteStudent(id)
      .then(() => { setMsg(`Étudiant ${id} supprimé.`); setSelected(null); load(); })
      .catch(() => setError('Erreur lors de la suppression'));
  };

  const cgpaColor = (v) => v >= 3.5 ? '#22c55e' : v >= 2.5 ? '#f59e0b' : '#ef4444';

  return (
    <div>
      <h1 className="page-title">Étudiants</h1>
      {error && <div className="error-banner">{error}</div>}
      {msg   && <div className="success-banner">{msg}</div>}

      <div className="toolbar">
        <span>Trier par :</span>
        {['cgpa','name','id'].map(by => (
          <button key={by} className={`chip ${sortBy === by ? 'active' : ''}`} onClick={() => handleSort(by)}>
            {by === 'cgpa' ? 'CGPA ↓' : by === 'name' ? 'Nom A→Z' : 'ID'}
          </button>
        ))}
        <span className="count">{students.length} étudiant(s)</span>
      </div>

      <div className="table-wrap">
        <table className="table">
          <thead>
            <tr><th>ID</th><th>Nom</th><th>Semestres</th><th>CGPA</th><th></th></tr>
          </thead>
          <tbody>
            {students.map(s => (
              <tr key={s.id} className={selected?.id === s.id ? 'row-selected' : ''} onClick={() => setSelected(s)}>
                <td><code>{s.id}</code></td>
                <td>{s.name}</td>
                <td>{s.semesters.length}</td>
                <td><span className="badge" style={{ background: cgpaColor(s.cgpa) }}>{s.cgpa.toFixed(2)}</span></td>
                <td><button className="btn-danger" onClick={e => { e.stopPropagation(); handleDelete(s.id); }}>✕</button></td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>

      {selected && (
        <div className="detail-panel">
          <div className="detail-header">
            <h2>{selected.name} <span className="id-tag">{selected.id}</span></h2>
            <span className="cgpa-big" style={{ color: cgpaColor(selected.cgpa) }}>
              CGPA {selected.cgpa.toFixed(2)}
            </span>
            <button className="close-btn" onClick={() => setSelected(null)}>✕</button>
          </div>
          {selected.semesters.map(sem => (
            <div key={sem.label} className="sem-block">
              <div className="sem-header">
                <strong>{sem.label}</strong>
                <span>GPA : {sem.gpa.toFixed(2)}</span>
              </div>
              <table className="table">
                <thead><tr><th>Cours</th><th>Crédits</th><th>Note</th><th>Points</th></tr></thead>
                <tbody>
                  {sem.courses.map(c => (
                    <tr key={c.name}>
                      <td>{c.name}</td>
                      <td>{c.credits}</td>
                      <td><code>{c.letterGrade}</code></td>
                      <td>{c.qualityPts.toFixed(1)}</td>
                    </tr>
                  ))}
                </tbody>
              </table>
            </div>
          ))}
        </div>
      )}
    </div>
  );
}
