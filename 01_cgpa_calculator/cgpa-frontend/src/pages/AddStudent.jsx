import { useState } from 'react';
import { api } from '../api';

const GRADES = ['A+','A','A-','B+','B','B-','C+','C','C-','D+','D','F'];

function CourseRow({ course, onChange, onRemove }) {
  return (
    <div className="course-row">
      <input className="input" placeholder="Nom du cours" value={course.name}
        onChange={e => onChange({ ...course, name: e.target.value })} />
      <input className="input input-sm" placeholder="Crédits" type="number" min="0.5" step="0.5"
        value={course.credits} onChange={e => onChange({ ...course, credits: parseFloat(e.target.value) })} />
      <select className="input input-sm" value={course.letterGrade}
        onChange={e => onChange({ ...course, letterGrade: e.target.value })}>
        {GRADES.map(g => <option key={g}>{g}</option>)}
      </select>
      <button className="btn-danger" onClick={onRemove}>✕</button>
    </div>
  );
}

const newCourse = () => ({ name: '', credits: 3, letterGrade: 'A' });
const newSem    = ()    => ({ label: '', courses: [newCourse()] });

export default function AddStudent({ onNavigate }) {
  const [id,       setId]       = useState('');
  const [name,     setName]     = useState('');
  const [semesters,setSemesters]= useState([newSem()]);
  const [error,    setError]    = useState('');
  const [success,  setSuccess]  = useState('');
  const [loading,  setLoading]  = useState(false);

  const updateSem = (si, patch) =>
    setSemesters(sems => sems.map((s, i) => i === si ? { ...s, ...patch } : s));

  const updateCourse = (si, ci, patch) =>
    setSemesters(sems => sems.map((s, i) => i !== si ? s : {
      ...s, courses: s.courses.map((c, j) => j === ci ? { ...c, ...patch } : c)
    }));

  const addCourse = (si) =>
    setSemesters(sems => sems.map((s, i) => i === si ? { ...s, courses: [...s.courses, newCourse()] } : s));

  const removeCourse = (si, ci) =>
    setSemesters(sems => sems.map((s, i) => i !== si ? s : {
      ...s, courses: s.courses.filter((_, j) => j !== ci)
    }));

  const handleSubmit = async () => {
    setError(''); setSuccess('');
    if (!id.trim() || !name.trim()) { setError("ID et nom requis."); return; }
    for (const sem of semesters) {
      if (!sem.label.trim()) { setError("Chaque semestre doit avoir un label."); return; }
      for (const c of sem.courses) {
        if (!c.name.trim()) { setError("Chaque cours doit avoir un nom."); return; }
      }
    }
    setLoading(true);
    try {
      await api.addStudent({ id: id.trim(), name: name.trim(), semesters });
      setSuccess(`Étudiant "${name}" ajouté avec succès !`);
      setId(''); setName(''); setSemesters([newSem()]);
    } catch (e) {
      setError(e.response?.data?.error || "Erreur lors de l'ajout.");
    } finally {
      setLoading(false);
    }
  };

  return (
    <div>
      <h1 className="page-title">Ajouter un étudiant</h1>
      {error   && <div className="error-banner">{error}</div>}
      {success && <div className="success-banner">{success} <button className="btn-link" onClick={() => onNavigate('Étudiants')}>Voir la liste →</button></div>}

      <div className="form-card">
        <div className="form-row">
          <div className="form-group">
            <label>ID étudiant</label>
            <input className="input" placeholder="ex: S010" value={id} onChange={e => setId(e.target.value)} />
          </div>
          <div className="form-group" style={{ flex: 2 }}>
            <label>Nom complet</label>
            <input className="input" placeholder="ex: Jean Dupont" value={name} onChange={e => setName(e.target.value)} />
          </div>
        </div>

        {semesters.map((sem, si) => (
          <div key={si} className="sem-block">
            <div className="sem-header">
              <input className="input" placeholder={`Label (ex: Semestre ${si + 1})`}
                value={sem.label} onChange={e => updateSem(si, { label: e.target.value })} />
              {semesters.length > 1 &&
                <button className="btn-danger" onClick={() => setSemesters(s => s.filter((_, i) => i !== si))}>Suppr. semestre</button>}
            </div>

            {sem.courses.map((c, ci) => (
              <CourseRow key={ci} course={c}
                onChange={patch => updateCourse(si, ci, patch)}
                onRemove={() => removeCourse(si, ci)} />
            ))}
            <button className="btn-secondary" onClick={() => addCourse(si)}>+ Ajouter un cours</button>
          </div>
        ))}

        <div className="form-actions">
          <button className="btn-secondary" onClick={() => setSemesters(s => [...s, newSem()])}>
            + Ajouter un semestre
          </button>
          <button className="btn-primary" onClick={handleSubmit} disabled={loading}>
            {loading ? 'Envoi…' : 'Enregistrer'}
          </button>
        </div>
      </div>
    </div>
  );
}
